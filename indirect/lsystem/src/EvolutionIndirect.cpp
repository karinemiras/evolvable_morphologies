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
void EvolutionIndirect::initPopulation(LSystem LS)
{

  //creates new genomes according to population size
  for (int i = 1; i <= this->params["pop_size"]; i++)
  {
    // initial genomes receive personal ids, but the ids of the parents are none
    Genome gen = Genome(std::to_string(this->next_id),
                        "N",
                        "N",
                        -1,
                        -1);


    // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
    gen.build_grammar(LS,
                      this->params);

    this->population.push_back(gen);  // adds genome to the population

    // increments id that will be used for any next genome created
    this->next_id++;

  }
}


/**
*   Performs crossover among individuals in the population.
**/

void EvolutionIndirect::crossover(
    LSystem LS,
    std::vector< Genome > &offspring)
{


  // creates new individuals via crossover (size of offspring is relative to the size of population)
  for (int i = 0;
       i < ceil(this->params["pop_size"] * this->params["offspring_prop"]); i++)
  {

    // selects parents for the crossover, according to INDEX
    int parent1 = this->tournament();
    int parent2 = parent1;

    while (parent2 == parent1)
    { // makes sure that parent2 is different from parent1
      parent2 = this->tournament();
    }

    // #TEST: Tests if selected parents are different.
    this->tests.testParents(parent1,
                            parent2);

    // creates new offspring genome
    Genome gen = Genome(std::to_string(this->next_id),
                        this->population[parent1].getId(),
                        this->population[parent2].getId(),
                        this->population[parent1].getFitness(),
                        this->population[parent2].getFitness());

    this->aux.logs(" crossover for genome " + std::to_string(this->next_id)
                   + " - p1: " + this->population[parent1].getId()
                   + " p2: " + this->population[parent2].getId());

    this->next_id++;

    std::map< std::string, GeneticString  > grammar =
        std::map< std::string, GeneticString  >();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_real_distribution< double > prob(0.0, 1.0);

    // for each letter in the grammar
    for (const auto &letter : LS.getAlphabet())
    {
      if (prob(generator) <= prob(generator))
      {
        this->aux.logs(letter.first + " from parent1");

        // copies object of rule from the genome parent1
        GeneticString gsp1 = GeneticString(this->population[parent1]
                                              .getGrammar()[letter.first]);
        grammar.emplace(letter.first, gsp1); // gets it from parent1
      }
      else
      {
        this->aux.logs(letter.first + " from parent2");

        // copies object of rule from the genome parent2
        GeneticString gsp2 = GeneticString(this->population[parent2]
                                               .getGrammar()[letter.first]);
        grammar.emplace(letter.first, gsp2); // gets it from parent1
      }
    }

    gen.setGrammar(grammar); // sets grammar for the new genome

    offspring.push_back(gen); // adds new individual to the offspring
  }

  // mutates new individuals
  this->mutation(LS, &offspring);
}


/**
 * Performs mutation to individuals of the offspring.
 * @param LS - Lsystem structure containing the alphabet.
 * @param offspring - offspring to be mutated
 */

void EvolutionIndirect::mutation(
    LSystem LS,
    std::vector< Genome > * offspring)
{


  std::random_device rd;
  std::default_random_engine generator(rd());

  // distribution for letters of the alphabet, to be possibly included
  // (does not include position 0, which is core-component, as it should be present only once)
  std::uniform_int_distribution< int >
      dist_letter(1,
                  (int) LS.getAlphabetIndex().size() - 1);

  // distribution for letters of the alphabet, to be the target of the mutation
  std::uniform_int_distribution< int >
      dist_letter_target(0,
                         (int) LS.getAlphabetIndex().size() - 1);

  // distribution for the mounting commands
  std::uniform_int_distribution< int >
      dist_mountingcommand(0,
                           (int) LS.getMountingCommands().size() - 1);

  // distribution for the moving commands
  std::uniform_int_distribution< int >
      dist_movingcommand(0,
                         (int) LS.getMovingCommands().size() - 1);

  // distribution for the brain move commands
  std::uniform_int_distribution< int >
      dist_brainmovecommand(0,
                            (int) LS.getBrainMoveCommands().size() - 1);

  // distribution for the brain change commands
  std::uniform_int_distribution< int >
      dist_brainchangecommand(0,
                              (int) LS.getBrainChangeCommands().size() - 1);

  // distribution for 0-1 probabilities
  std::uniform_real_distribution< double > prob(0.0,
                                                1.0);


  // for each genome of the offspring
  for (int i = 0; i < offspring->size(); i++)
  {

    std::string mutate_letter = LS.getAlphabetIndex()[dist_letter_target(generator)];
    // distribution for the type of mutation which will be performed
    std::uniform_int_distribution< int > type_mutation(1,
                                                       3);
    int type_of_mutation = type_mutation(generator);


    // mutates genome given  probability
    if (prob(generator) <= this->params["mutation_prob"])
    {

      // # deletion
      if (type_of_mutation == 1)
      {

        //  if there is at least more than one component
        if (offspring->at(i).getGrammar()[mutate_letter].count() > 1)
        {

          // distribution for position of deletion in the genetic-string
          std::uniform_int_distribution< int > pos_d(1,
                                                     offspring->at(i).getGrammar()[mutate_letter].count());
          int pos_deletion = pos_d(generator);

          // if it is the production rule of the core-component, prevents core-component from being deleted, preserving the root
          if (mutate_letter == "C" and pos_deletion == 1)
          {
            // std::cout<<"cant delete the the core-component from the beginning of its rule";
          }
          else
          {

            this->aux.logs("mutation: remove in " + offspring->at(i).getId()
                           + " for " + mutate_letter
                           + " at " + std::to_string(pos_deletion));
            offspring->at(i).getGrammar()[mutate_letter].remove(pos_deletion); //
            // removes item from chosen position

          }
        }

        // # swapping
      }
      else if (type_of_mutation == 2)
      {


        // distribution for position of insertion/swap in the genetic-string
        std::uniform_int_distribution< int > pos_s(1,
                                                   offspring->at(i).getGrammar()[mutate_letter].count());

        // position of items to be swapped in the genetic-string
        int pos_swap1 = pos_s(generator);
        int pos_swap2 = pos_s(generator);

        while (pos_swap2 < pos_swap1)
        {
          pos_swap2 = pos_s(generator);
        }  // makes sure position swap1 is before position swap2

        // never swaps core component
        if (mutate_letter == "C" and pos_swap1 == 1)
        {
          pos_swap1 = 0;
          pos_swap2 = 0;
        }

        offspring->at(i).getGrammar()[mutate_letter].swap(pos_swap1,
                                                       pos_swap2); // removes item from chosen position
        this->aux.logs("mutation: swap in " + offspring->at(i).getId() + " for " +
                       mutate_letter
                       + " between " + std::to_string(pos_swap1) + " and " +
                       std::to_string(pos_swap2));


        // # adding
      }
      else if (type_of_mutation == 3)
      {

        // type of item to add
        std::uniform_int_distribution< int > type_adding(1,
                                                         5);
        int type_of_adding = type_adding(generator);
        int aux =0;

        std::string genetic_string_item = "";

        // distribution for position of insertion in the genetic-string
        std::uniform_int_distribution< int > pos_i(0,
                                                   offspring->at(i).getGrammar()[mutate_letter].count());
        int pos_insertion = pos_i(generator);

        // if it is the production rule of the core-component, prevents new items from being inserted at the beginning, preserving the root
        if (mutate_letter == "C" and pos_insertion == 0)
        {
          pos_insertion++;
        }

        // # adding mounting command
        if (type_of_adding == 1)
        {
          aux = dist_mountingcommand(generator);
          this->aux.logs(
              "mutation: add mounting command " + LS.getMountingCommands()[aux]
              + " in " + offspring->at(i).getId()
              + " for " + mutate_letter + " at "
              + std::to_string(pos_insertion));
          genetic_string_item = LS.getMountingCommands()[aux];
        }

          // # adding letter
        else if (type_of_adding == 2)
        {
          aux = dist_letter(generator);
          auto letter = LS.buildBrainCommand(LS.getAlphabetIndex()[aux]);
          this->aux.logs("mutation: add letter " + letter
                         + " in " + offspring->at(i).getId()
                         + " for " + mutate_letter + " at "
                         + std::to_string(pos_insertion));
          genetic_string_item = letter;
        }

          // # adding moving command
        else if (type_of_adding == 3)
        {
          aux = dist_movingcommand(generator);
          this->aux.logs("mutation: add moving command " + LS
              .getMovingCommands()[aux]
                         + " in " + offspring->at(i).getId()
                         + " for " + mutate_letter
                         + " at " + std::to_string(pos_insertion));
          genetic_string_item = LS.getMovingCommands()[aux];
        }

          // # adding brain move command
        else if (type_of_adding == 4)
        {
          aux = dist_brainmovecommand(generator);
          std::string com = LS.buildBrainCommand(LS.getBrainMoveCommands()[aux]);
          this->aux.logs("mutation: add brain move command "
                         + com
                         + " in " + offspring->at(i).getId()
                         + " for " + mutate_letter
                         + " at " + std::to_string(pos_insertion));
          genetic_string_item = com;
        }

          // # adding brain change command
        else if (type_of_adding == 5)
        {
          aux = dist_brainchangecommand(generator);
          std::string com = LS.buildBrainCommand(LS.getBrainChangeCommands()[aux]);
          this->aux.logs("mutation: add brain change command "
                         + com
                         + " in " + offspring->at(i).getId()
                         + " for " + mutate_letter
                         + " at " + std::to_string(pos_insertion));
          genetic_string_item = com;
        }

        //  (possibly) alters genetic-string (production rule) adding items (letters or commands)
        offspring->at(i).getGrammar()[mutate_letter].add(pos_insertion,
                                                      genetic_string_item);
      }
    }
  }
}
