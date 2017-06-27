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

using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // EuclideanDistance

#include "Aux.h"
#include "EvolutionDirect.h"
#include "Genome.h"
#include "LSystem.h"
#include "Measures.h"





/**
 * Initializes the population with new genomes.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void EvolutionDirect::initPopulation(LSystem LS){ // default arguments and Lsystem

    //creates new genomes according to population size
    for(int i=1; i <= this->params["pop_size"]; i++) {

        // initial genomes receive personal ids, but the ids of the parents are none
        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N", -1, -1);

        // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
        gen->build_genome_direct(LS, this->params);

        this->population->push_back(gen);  // adds genome to the population

        // increments id that will be used for any next genome created
        this->next_id++;
    }

}




/**
*   Performs crossover among individuals in the population.
**/

void EvolutionDirect::crossover(LSystem LS, std::vector<Genome *>  * offspring){


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

        std::random_device rd;
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<double> prob(0.0, 1.0); // distribution for probabilities


        std::uniform_int_distribution<int> dist_pos_parent1_ini(2,
                                                                this->population->at(parent1)->getGeneticString().count()); // distribution for parent1 initial position, skips first (core-component)
        std::uniform_int_distribution<int> dist_pos_parent2_ini(2,
                                                                this->population->at(parent2)->getGeneticString().count()); // distribution for parent2 initial position, skips first (core-component)


        // raffles random positions for the start point (of the genetic string) for both parents, if the genetic string contains more than the core-component
        int pos_parent1_ini = 0,  pos_parent2_ini = 0;

        if(this->population->at(parent1)->getGeneticString().count() > 1) {

            pos_parent1_ini =  dist_pos_parent1_ini(generator);
        }else{
            pos_parent1_ini = 0;
        }

        if(this->population->at(parent2)->getGeneticString().count() > 1) {

            pos_parent2_ini =  dist_pos_parent2_ini(generator);
        }else{
            pos_parent2_ini = 0;
        }


        std::uniform_int_distribution<int> dist_pos_parent1_end(pos_parent1_ini,
                                                                this->population->at(parent1)->getGeneticString().count()); // distribution for parent1 final position
        std::uniform_int_distribution<int> dist_pos_parent2_end(pos_parent2_ini,
                                                                this->population->at(parent2)->getGeneticString().count()); // distribution for parent2 final position

        int pos_parent1_end = 0, pos_parent2_end = 0;

        // if theres more than one item forming the genetic string for the parent

        if(this->population->at(parent1)->getGeneticString().count()>1) {

            pos_parent1_end = dist_pos_parent1_end(generator);
        }else{
            pos_parent1_end = 0;
        }

        if(this->population->at(parent2)->getGeneticString().count()>1) {

            pos_parent2_end = dist_pos_parent2_end(generator);
        }else{
            pos_parent2_end = 0;
        }

        GeneticString gs;

       // std::cout<<" ------- pos "<<pos_parent1_ini<<" "<<pos_parent1_end<<" "<<pos_parent2_ini<<" "<<pos_parent2_end <<std::endl;

        gs.create_joined_list(pos_parent1_ini, pos_parent2_ini,
                              pos_parent1_end, pos_parent2_end,
                              this->population->at(parent1)->getGeneticString(),
                              this->population->at(parent2)->getGeneticString());



        gs.display_list();

        gen->setGeneticString(gs); // sets genetic string for the new genome

        offspring->push_back(gen); // adds new individual to the offspring

    }


    this->mutation(LS, offspring); // mutates new individuals

}


/**
 * Performs mutation to individuals of the offspring.
 * @param LS - Lsystem structure containing the alphabet.
 * @param offspring - offspring to be mutated
 */

void EvolutionDirect::mutation(LSystem LS, std::vector<Genome *> * offspring) {

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


        //  if there is at least more than two components, and if the raffled probability is within the constrained probability
        if (offspring->at(i)->getGeneticString().count() >= 2 and prob(generator) < this->params["mutation_delete_prob"]) {

            // distribution for position of deletion in the genetic-string, starting from second position, preserving core-component
            std::uniform_int_distribution<int> pos_d(2, offspring->at(i)->getGeneticString().count());
            int pos_deletion = pos_d(generator);

            this->aux.logs("mutation: remove in "+ offspring->at(i)->getId());
            offspring->at(i)->getGeneticString().remove(pos_deletion); // removes item from chosen position

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

        // distribution for position of insertion in the genetic-string, from second position on, leaving the first for the core-component
        std::uniform_int_distribution<int> pos_i(1,  offspring->at(i)->getGeneticString().count());
        int pos_insertion = pos_i(generator);

        //  (possibly) alters genetic-string (production rule) adding items (letters or moving commands).
        offspring->at(i)->getGeneticString().add(pos_insertion, genetic_string_items);

    }


}

