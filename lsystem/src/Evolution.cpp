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
            this->params[tokens[0]] = std::stod(tokens[1]);
        }
        myfile.close();
    }

    else std::cout << "Unable to open parameters file."<<std::endl;

}


/**
 * Initializes the population with genomes.
 */
void Evolution::initPopulation(int argc, char* argv[], LSystem LS){

    for(int i=1; i <= this->params["pop_size"]; i++) {

        std::cout<<" ------ genome "<<i<<std::endl;
        Genome * gen = new Genome(std::to_string(i));
        gen->createEmbryo();
        gen->developGenome(argc, argv, this->params, LS);


        gen->measurePhenotype();

        this->population.push_back(gen);
    }

}



