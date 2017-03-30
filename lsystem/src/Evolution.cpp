//
// Created by Karine Miras on 21/03/2017.
//
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "Evolution.h"
#include "Genome.h"
#include "LSystem.h"

/**
 * Reads parameters for evolution.
 **/
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
 **/
void Evolution::initPopulation(int argc, char* argv[], LSystem LS){

    for(int i=1; i <= this->params["pop_size"]; i++) {

        std::cout<<" ------ genome "<<i<<std::endl;
        Genome * gen = new Genome(std::to_string(i));
        gen->createEmbryo();  // creates initial genetic string with axiom
        gen->developGenome(argc, argv, this->params, LS);  // grows genetic-string according to grammar, decodes it and constructs the phenotype
        this->population.push_back(gen);
    }

}


/**
 * Measures all individuals in the population.
 **/
 void Evolution::measurePopulation(){

    std::ofstream measures_file_general;
    std::string path = "/Users/karinemiras/CLionProjects/lsystem-proto/tests/measures.txt";
    measures_file_general.open(path);

    for(int i=0; i <= this->params["pop_size"]-1; i++) {
        this->population[i]->measurePhenotype(this->params);
    }
    measures_file_general.close();

}



/**
 * Loads population of genomes from files.
 **/

void Evolution::loadPopulation(int argc, char* argv[],int size_pop, std::string test_folder, LSystem LS){

    std::string line;
    std::stringstream convert;

    for (int i=1; i<=size_pop; i++) { // for each genome file in the specified folder, reads its contents

        convert << i;
        std::ifstream myfile("/Users/karinemiras/CLionProjects/lsystem-proto/" + test_folder + "/genome_" + convert.str() + ".txt");
        if (myfile.is_open()) {
            getline(myfile, line);
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(" "));

            for(int j=0; j<tokens.size(); j++){

            }

            myfile.close();
        }else{
            std::cout<<"Cant open genome "<<i;
        }
    }

};


/**
 * Test a genome made from a had-designed genetic-string.
 */

void Evolution::testGeneticString(int argc, char* argv[],std::string test_genome, LSystem LS) {

    std::string line;
    std::ifstream myfile("/Users/karinemiras/CLionProjects/lsystem-proto/" + test_genome);
    if (myfile.is_open()) {
        getline(myfile, line);
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" "));
        std::vector<std::string> gs;

        for(int j=0; j<tokens.size(); j++){ // for each item in the genetic-string
            if(tokens[j] != "") { gs.push_back(tokens[j]); }
        }

        Genome * gen = new Genome("1");
        gen->setGeneticString(gen->build_genetic_string(gen->getGeneticString(), gs));
        gen->getGeneticString().display_list();

        // decodes the final genetic-string into a tree of components
        std::cout << " >> decoding ... " << std::endl;
        gen->decodeGeneticString(LS);

        // generates robot-graphics
        std::cout << " >> constructing ... " << std::endl;
        gen->constructor(argc, argv, this->params);

        gen->measurePhenotype(this->params);

        myfile.close();
    }else{
        std::cout<<"Cant open genome.";
    }
}

