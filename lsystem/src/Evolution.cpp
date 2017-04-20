//
// Created by Karine Miras on 21/03/2017.
//

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "Evolution.h"
#include "Genome.h"
#include "LSystem.h"
#include "Measures.h"



/**
 * Reads parameters from file.
 **/
void Evolution::readParams(){

    std::string line;
    std::ifstream myfile ("../../lsystem/configuration.txt");

    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            std::vector<std::string> tokens;
            boost::split( tokens, line, boost::is_any_of(" ") );  // parameters label and value separated by space

            this->params[tokens[0]] = std::stod(tokens[1]);  // first item is the label, second is the value
        }
        myfile.close();
    }

    else std::cout << "Unable to open parameters file."<<std::endl;

}


/**
 * Initializes the population with new genomes.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void Evolution::initPopulation(int argc, char* argv[], LSystem LS){ // default arguments and Lsystem

    for(int i=1; i <= this->params["pop_size"]; i++) {

        std::cout<<" ------ genome "<<i<<std::endl;
        Genome * gen = new Genome(std::to_string(i));

        gen->createEmbryo();  // creates main genetic-string for axiom (initial developmental state of the genome)

        std::cout << " >> building grammar ..." << std::endl;
        gen->build_grammar(LS, this->params["num_initial_comp"], this->params["add_backtoparent_prob"]); // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)

        gen->developGenome(argc, argv, this->params, LS);  // 1- grows genetic-string of teh genome according to grammar, 2- decodes it, 3- constructs the phenotype

        this->population.push_back(gen);  // add genome to the population
    }

}


/**
 * Measures all the individuals of the population for several metrics.
 **/
 void Evolution::measurePopulation(int argc, char* argv[]){

    std::ofstream measures_file_general;
    std::string path = "../../tests/measures.txt";
    measures_file_general.open(path);

    for(int i=0; i < this->params["pop_size"]; i++) {  // for each genome of the population

        Measures * m = new Measures();
        m->setGenome(this->population[i]);
        m->measurePhenotype(argc, argv,this->params);
    }
    measures_file_general.close();

}



/**
 * Loads population of genomes from files.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param size_pop - size of the population to be loaded.
 * @param test_folder - the path with folder/file from where to read the population files.
 * @param LS - Lsystem structure containing the alphabet.
 **/

void Evolution::loadPopulation(int argc, char* argv[],int size_pop, std::string test_folder, LSystem LS){

    std::string line;
    std::stringstream convert;

    for (int i=1; i<=size_pop; i++) { // for each genome file in the specified folder, reads its contents

        convert << i; // if of genome
        std::ifstream myfile("../../tests/" + test_folder + "/genome_" + convert.str() + ".txt");
        if (myfile.is_open()) {
            getline(myfile, line);
            std::vector<std::string> tokens;
            boost::split(tokens, line, boost::is_any_of(" ")); // items of the genetic-string separated by space

            for(int j=0; j<tokens.size(); j++){
                // build genetic string vector!
            }

            myfile.close();
        }else{
            std::cout<<"Cant open genome "<<i;
        }
    }

};


/**
 * Test a genome made from a had-designed genetic-string.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param test_genome - the path with folder/file from where to read the genome to be tested.
 * @param LS - Lsystem structure containing the alphabet.
 */

void Evolution::testGeneticString(int argc, char* argv[],std::string test_genome, LSystem LS) {

    std::string line;
    std::ifstream myfile("../../fixed_morph/" + test_genome+ ".txt");
    if (myfile.is_open()) {
        getline(myfile, line);
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(" ")); // items of the genetic-string separated by space
        std::vector<std::string> gs;

        for(int j=0; j<tokens.size(); j++){ // adds each item in the genetic-string in the array of items
            if(tokens[j] != "") { gs.push_back(tokens[j]); }
        }

        // creates new genome with id equal 1, using the just read genetic-string
        Genome * gen = new Genome(test_genome);
        gen->setGeneticString(gen->build_genetic_string(gen->getGeneticString(), gs));
        gen->getGeneticString().display_list();

        // decodes the final genetic-string into a tree of components
        std::cout << " >> decoding ... " << std::endl;
        gen->decodeGeneticString(LS);

        // generates robot-graphics
        std::cout << " >> constructing ... " << std::endl;
        gen->constructor(argc, argv, this->params);

        // measures all metrics od the genome
        Measures m;
        m.setGenome(gen);
        m.measurePhenotype(argc, argv, this->params);

        myfile.close();
    }else{
        std::cout<<"Cant open genome.";
    }
}

