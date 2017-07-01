//
// Created by Karine Miras on 21/03/2017.
//

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

#include "Aux.h"
#include "EvolutionIndirect.h"
#include "Genome.h"
#include "LSystem.h"
#include "Measures.h"





/**
 * Initializes the population with new genomes.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void EvolutionIndirect::initPopulation(LSystem LS){ // default arguments and Lsystem

    //creates new genomes according to population size
    for(int i=1; i <= this->params["pop_size"]; i++) {

        // initial genomes receive personal ids, but the ids of the parents are none
        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N", -1, -1);

        // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
        gen->build_grammar(LS, this->params);

        this->population->push_back(gen);  // adds genome to the population

        // increments id that will be used for any next genome created
        this->next_id++;
    }

}




/**
*   Performs crossover among individuals in the population.
**/

void EvolutionIndirect::crossover(LSystem LS, std::vector<Genome *>  * offspring){



    // creates new individuals via crossover (size of offspring is relative to the size of population)
    for(int i = 0; i < ceil(this->params["pop_size"] * this->params["offspring_size"]); i++) {

        // selects parents for the crossover, according to INDEX
        int parent1 = this->tournament();
        int parent2 = parent1;

        while(parent2 == parent1){ // makes sure that parent2 is different from parent1
            parent2 = this->tournament();
        }

        // #TEST: Tests if selected parents are different.
        this->tests.testParents(parent1, parent2);

        // creates new offspring genome
        Genome * gen = new Genome(std::to_string(this->next_id),
                                  this->population->at(parent1)->getId(),
                                  this->population->at(parent2)->getId(),
                                  this->population->at(parent1)->getFitness(),
                                  this->population->at(parent2)->getFitness());

        this->aux.logs(" crossover for genome " + std::to_string(this->next_id) + " - p1: " + this->population->at(parent1)->getId() + " p2: " + this->population->at(parent2)->getId());

        this->next_id++;

        std::map< std::string, GeneticString >  grammar = std::map< std::string, GeneticString >();

        std::random_device rd;
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<double> prob(0.0, 1.0); // distribution for probabilities

        // for each letter in the grammar
        for ( auto &it : LS.getAlphabet()) {

            if(prob(generator) <= prob(generator)) {

                grammar.emplace(it.first, this->population->at(parent1)->getGrammar()[it.first]); // gets is from parent1
            } else{

                grammar.emplace(it.first, this->population->at(parent2)->getGrammar()[it.first]); // gets is from parent2
            }


            // grammar[it.first].display_list();

        }

        gen->setGrammar(grammar); // sets grammar for the new genome

        offspring->push_back(gen); // adds new individual to the offspring

    }


    this->mutation(LS, offspring); // mutates new individuals

}


/**
 * Performs mutation to individuals of the offspring.
 * @param LS - Lsystem structure containing the alphabet.
 * @param offspring - offspring to be mutated
 */

void EvolutionIndirect::mutation(LSystem LS, std::vector<Genome *> * offspring) {


    std::random_device rd;
    std::default_random_engine generator(rd());

    // distribution for letters of the alphabet (does not include position 0, which is core-component, as it should be present only once)
    std::uniform_int_distribution<int> dist_letter(1, (int) LS.getAlphabetIndex().size()-1);

    // distribution for the mounting commands
    std::uniform_int_distribution<int> dist_mountingcommand(0, (int) LS.getMountingCommands().size()-1);

    // distribution for the moving commands
    std::uniform_int_distribution<int> dist_movingcommand(0, (int) LS.getMovingCommands().size()-1);

    // distribution for 0-1 probabilities
    std::uniform_real_distribution<double> prob(0.0, 1.0);


    for(int i=0; i < offspring->size(); i++) {  // for each genome of the offspring


        for ( auto &it : offspring->at(i)->getGrammar()) { // for each letter in the grammar


            //  if there is at least more than two components, and if the raffled probability is within the constrained probability
            if (it.second.count() >= 2 and prob(generator) < this->params["mutation_delete_prob"]) {

                // distribution for position of deletion in the genetic-string
                std::uniform_int_distribution<int> pos_d(1, it.second.count());
                int pos_deletion = pos_d(generator);

                // if it is the production rule of the core-component, prevents core-component from being deleted, preserving the root
                if(it.first == "C" and pos_deletion == 1){
                      // std::cout<<"cant delete the the core-component from the beginning of its rule";
                }else{
                    this->aux.logs("mutation: remove in "+ offspring->at(i)->getId());
                    it.second.remove(pos_deletion); // removes item from chosen position
                }
            }

            std::vector<std::string> genetic_string_items = std::vector<std::string>();

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_add_prob"]) {

                // raffles a command to add
                this->aux.logs("mutation: add mounting command in " + offspring->at(i)->getId());
                genetic_string_items.push_back(LS.getMountingCommands()[dist_mountingcommand(generator)]);
            }

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_add_prob"]) {

                // raffles a letter to add
                this->aux.logs("mutation: add letter in " + offspring->at(i)->getId());
                genetic_string_items.push_back(LS.getAlphabetIndex()[dist_letter(generator)]);
            }

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_move_prob"]) {

                // adds moving command
                this->aux.logs("mutation: add moving command in " + offspring->at(i)->getId());
                genetic_string_items.push_back(LS.getMovingCommands()[dist_movingcommand(generator)]);
            }


//            // if raffled probability is within the constrained probability
//            if (prob(generator) < this->params["mutation_delete_swap"]) {
//
//                // distribution for position of insertion/swap in the genetic-string
//                std::uniform_int_distribution<int> pos_s(1, it.second.count());
//
//                // position of items to be swapped in the genetic-string
//                int pos_swap1 = pos_s(generator);
//                int pos_swap2 = pos_s(generator);
//                int aux = 0;
//
//                if(it.first == "C" and pos_swap1 == 1) pos_swap1 = 2;
//                if(it.first == "C" and pos_swap2 == 1) pos_swap2 = 2;
//
//                // makes sure position swap1 is before position swap2
//                if(pos_swap1 > pos_swap2) {
//                    aux = pos_swap1;
//                    pos_swap2 = pos_swap1;
//                    pos_swap1 = aux;
//                }
//
//                it.second.swap(pos_swap1, pos_swap2); // removes item from chosen position
//            }


            // distribution for position of insertion/swap in the genetic-string
            std::uniform_int_distribution<int> pos_i(0, it.second.count());
            int pos_insertion = pos_i(generator);

            // if it is the production rule of the core-component, prevents new items from being inserted at the beginning, preserving the root
            if(it.first == "C" and pos_insertion == 0) pos_insertion++;

            //  (possibly) alters genetic-string (production rule) adding items (letters or moving commands).
            it.second.add(pos_insertion, genetic_string_items);

        }

    }

}

