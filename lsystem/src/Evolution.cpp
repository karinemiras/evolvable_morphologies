//
// Created by Karine Miras on 21/03/2017.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
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
 *  @param argc - command line parameter
 *  @param argv[] - command line parameter
 **/
 void Evolution::measurePopulation(int argc, char* argv[]){

    std::ofstream measures_file_general;
    std::string path = "../../tests/measures.txt";
    measures_file_general.open(path);

    for(int i=0; i < this->population.size(); i++) {  // for each genome of the population

        Measures * m = new Measures();
        m->setGenome(this->population[i]);
        m->measurePhenotype(argc, argv,this->params);
    }
    measures_file_general.close();

}


/**
 * Initializes the averages of the measures of the population with zero.
 */

void Evolution::initPop_measures(){


    this->pop_measures.emplace("total_components", 0); //  total amount of components of all types  in the body
    this->pop_measures.emplace("total_bricks", 0); //  total amount of brick-components
    this->pop_measures.emplace("total_fixed_joints_horizontal", 0); //  total amount of horizontal fixed-joint-components
    this->pop_measures.emplace("total_passive_joints_horizontal", 0); // measure: total amount of horizontal passive-joint-components
    this->pop_measures.emplace("total_active_joints_horizontal", 0); //   total amount of horizontal active-joint-components
    this->pop_measures.emplace("total_fixed_joints_vertical", 0); //  total amount of vertical fixed-joint-components
    this->pop_measures.emplace("total_passive_joints_vertical", 0); // measure: total amount of vertical passive-joint-components
    this->pop_measures.emplace("total_active_joints_vertical", 0); //   total amount of vertical active-joint-components
    this->pop_measures.emplace("connectivity1", 0); //   total of components with one side connected to another component
    this->pop_measures.emplace("connectivity2",0); //  total of components with two sides connected to another component
    this->pop_measures.emplace("connectivity3", 0); // total of components with three sides connected to another component
    this->pop_measures.emplace("connectivity4", 0); //   total of components with four sides connected to another component
    this->pop_measures.emplace("effective_joints", 0); //  total of joints connected by both sides to a brick or core component
    this->pop_measures.emplace("effective_ap_h_joints", 0); //  total of horizontal- active/passive- joints connected by both sides to a brick or core component 
    this->pop_measures.emplace("viable_joints", 0); //  total of effective joints which have no neighboards preventing movement
    this->pop_measures.emplace("length_ratio", 0); // length of the shortest side dived by the longest
    this->pop_measures.emplace("coverage", 0); // proportion of the expected area (given the horizontal e vertical lengths) that is covered with components
    this->pop_measures.emplace("spreadness", 0); // average distance of each component from each other in the axises x/y
    this->pop_measures.emplace("horizontal_symmetry",  0); // proportion of components in the left side which match with the same type of component in the same relative position on the right side
    this->pop_measures.emplace("vertical_symmetry", 0); // proportion of components in the top side which match with the same type of component in the same relative position on the bottom side
    this->pop_measures.emplace("joints_per_limb", 0); //  total amount of effective joints per limb
}



/**
 * Calculates the fitness of the population of genomes.
 */

void Evolution::evaluatePopulation(){

    for(int i=0; i < this->population.size(); i++) {  // for each genome of the population

        this->population[i]->calculateFitness(this->pop_measures); // calculates its fitness
        std::cout<<"fitness "<<this->population[i]->getFitness()<<std::endl;
    }
    this->updatePop_measures();  // updates the average measures for the population
}


/**
 * Calculates the average of the measures among all genomes.
 */

void Evolution::updatePop_measures(){

    for( const auto& it : this->pop_measures ){  // for each measure

        this->pop_measures[it.first] = 0;  // cleans the old value

        for(int i=0; i < this->population.size(); i++) {

            this->pop_measures[it.first] += this->population[i]->getMeasures()[it.first]; // sums the values of all individuals for the measure
        }

        this->pop_measures[it.first] /= (double)this->population.size(); // divides by the total
    }
}


/**
 * Selects two random genomes and compares their fitness, choosing the winner.
 * @return - the index of the winner genome
 */

int Evolution::tournament(){

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> dist_1(0, this->population.size()-1);

    int genome1 =  dist_1(generator);
    int genome2 =  dist_1(generator);

    if (this->population[genome1]->getFitness() > this->population[genome2]->getFitness()){ // if genome1 has a better fitness, it is selected
        return genome1;
    }else{
        return genome2;
    }
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

/**
*  Selection of genomes in a population.
**/

void Evolution::selection() {

    std::vector<Genome *>  selected = std::vector<Genome *>();

    for(int i=0; i < this->params["pop_size"]; i++) { // selects a subset of fit genomes

        selected.push_back(this->population[this->tournament()]);
    }

    this->population = selected; // substitutes current population for the selected subset
}


/**
*   Performs crossover among individuals in the population.
**/

void Evolution::crossover(){

    for(int i=0; i < this->params["offspring_size"]; i++) { // creates new individuals via crossover

        int parent1 = this->tournament();
        int parent2 = this->tournament();

        this->population.push_back(this->population[parent1]); // temp !!! substitute parent1 for offspring !!
    }
 }



std::vector<Genome *> Evolution::getPopulation(){

    return this->population;
}