//
// Created by Karine Miras on 21/03/2017.
//
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "Evolution.h"
#include "Genome.h"
#include "LSystem.h"

/**
 * Reads parameters for evolution.
 */
void Evolution::readParams(){

    std::string line;
    std::ifstream myfile ("/Users/karinemiras/CLionProjects/lsystem-proto/lsystem/configuration.txt");
    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            std::vector<std::string> tokens;
            boost::split( tokens, line, boost::is_any_of(" ") );
            this->params[tokens[0]] = std::stoi(tokens[1]);
        }
        myfile.close();
    }

    else std::cout << "Unable to open parameters file."<<std::endl;

}


/**
 * Initializes the population with genomes.
 */
void Evolution::initPopulation(int argc, char* argv[], LSystem LS){

    for(int i=0; i <= params["pop_size"]; i++) {

        std::cout<<" ------ genome "<<i<<std::endl;
        std::vector<std::string> axiom;
        axiom.push_back("CNNN");

        Genome gen(std::to_string(i+1), axiom);
        // initializes the genetic-string with the axiom
        gen.setGeneticString(gen.build_genetic_string(gen.getGeneticString(), gen.getAxiom()));

        std::cout << " >> building axiom ..." << std::endl;
        gen.getGeneticString().display_list();

        // creates genetic-strings for production initial rules with the grammar
        std::cout << " >> building grammar ..." << std::endl;
        gen.build_grammar(LS);

        // enhances the genetic-string according to grammar iteratively
        std::cout << " >> iterating replacements ..." << std::endl;
        gen.generate_final_string();

        // decodes the final genetic-string into a tree of components
        std::cout << " >> decoding ... " << std::endl;
        gen.decodeGeneticString(LS);

        // generates robot-graphics
        std::cout << " >> constructing ... " << std::endl;
        gen.constructor(argc, argv);

        this->population.push_back(gen);
    }

}