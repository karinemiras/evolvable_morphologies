////
//// Created by Karine Miras on 21/03/2017.
////
//
//#include <algorithm>
//#include <ctime>
//#include <fstream>
//#include <iostream>
//#include <map>
//#include <random>
//#include <string>
//#include <sstream>
//#include <vector>
//
//#include <boost/algorithm/string/classification.hpp>
//#include <boost/algorithm/string/split.hpp>
//
//#include <mlpack/core.hpp>
//#include <mlpack/methods/neighbor_search/neighbor_search.hpp>
//
//#include "Aux.h"
//#include "EvolutionIndirect.h"
//#include "Genome.h"
//#include "LSystem.h"
//#include "Measures.h"
//
//
//
//
//
///**
// * Initializes the population with new genomes.
// * @param LS - Lsystem structure containing the alphabet.
// **/
//void EvolutionIndirect::initPopulation(LSystem LS){ // default arguments and Lsystem
//
//    //creates new genomes according to population size
//    for(int i=1; i <= this->params["pop_size"]; i++) {
//
//        // initial genomes receive personal ids, but the ids of the parents are none
//        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N", -1, -1);
//
//        // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
//       // gen->build_grammar(LS, this->params);
//
//        this->population->push_back(gen);  // adds genome to the population
//
//        // increments id that will be used for any next genome created
//        this->next_id++;
//    }
//
//}
//
//
//
