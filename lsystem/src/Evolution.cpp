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

    /*   pop_size - size of the population of genomes
    *    add_backtoparent_prob - probability of adding a back-to-parent command to genetic-strings
    *    num_initial_comp - number of initial (random) components in the production rules of the grammar
    *    show_phenotypes - flag to show the phenotype graphic
    *    export_phenotypes - flag to export the phenotypes to  images
    *    export_genomes - flag to export the genomes to files
    *    replacement_iterations - number of replacement iterations for the l-system
    *    size_component - size of each component in pixels
    *    spacing - spacing between components in pixels
    *    offspring_size - proportion of the size of the population to calculate the number of individuals of the offsfpring
    *    num_generations - number of generations of the evolution
    *    mutation_alter_prob - probability of adding/removing items (letters/commands) to the genetic-string in the mutation
    *    max_comps - maximum number of components allowed per phenotype
    *    prop_parent - proportion from each parent in the crossover
    */

    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            std::vector<std::string> tokens;

            // parameters label and value separated by space
            boost::split( tokens, line, boost::is_any_of(" ") );

            // first item is the label, second is the value
            this->params[tokens[0]] = std::stod(tokens[1]);
        }
        myfile.close();
    }

    else std::cout << "Unable to open parameters file."<<std::endl;

}


/**
 * Initializes the population with new genomes.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void Evolution::initPopulation(LSystem LS){ // default arguments and Lsystem

    // repeats according to population size
    for(int i=1; i <= this->params["pop_size"]; i++) {

        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N");

        // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
        gen->build_grammar(LS, (int) this->params["num_initial_comp"], this->params["add_backtoparent_prob"]);

        this->population.push_back(gen);  // adds genome to the population

        this->next_id++;
    }

}



/**
*  Develops genomes of the population: 1- grows genetic-string of the genome according to grammar, 2- decodes it, 3- constructs the phenotype
*  @param argc - default argument
*  @param argv[] - default argument
*  @param LS - Lsystem structure containing the alphabet
*  @param individuals - array with genomes
**/
void Evolution::developIndividuals(int argc, char* argv[], LSystem LS, int generation, std::vector<Genome *> * individuals, std::string path){

    for(int i=0; i < individuals->size(); i++) {

        individuals->at(i)->developGenome(argc, argv, this->params, LS,  generation, path);  // develops genome
    }
}


/**
*  Constructs the phenotype and exports it.
*  @param argc - default argument
*  @param argv[] - default argument
*  @param LS - Lsystem structure containing the alphabet
*  @param generation - number of generation in evolution
*  @param path - directory to where export the phenotypes
**/
void Evolution::exportPop(int argc, char* argv[], LSystem LS, int generation, std::string path){

    for(int i=0; i < population.size(); i++) { // for each genome

        population[i]->constructor(argc, argv, this->params,  generation, path); // generates
    }
}



/**
 * Measures all the individuals of the population for several metrics.
 *  @param argc - command line parameter
 *  @param argv[] - command line parameter
 *  generation - number of the generation
 *  individuals - array with genomes
 **/
 void Evolution::measureIndividuals(int generation, std::vector<Genome *>  * individuals, std::string dirpath){

    for(int i=0; i < individuals->size(); i++) {  // for each genome of the population

        Measures * m = new Measures();
        m->setGenome(individuals->at(i));
        m->measurePhenotype(this->params, generation, dirpath);
    }
}



/**
 * Calculates the fitness of the population of genomes.
 */

void Evolution::evaluateIndividuals(std::vector<Genome *> * individuals){

    // for each genome of the population
    for(int i=0; i < individuals->size(); i++) {

        // calculates its fitness
        individuals->at(i)->calculateFitness(this->pop_measures);
        std::cout<<"fitness genome "<<individuals->at(i)->getId()<<" : "<<individuals->at(i)->getFitness()<<std::endl;
    }
}


/**
 * Calculates the average of the measures among all genomes.
 */

void Evolution::updatePop_measures_average(std::vector<Genome *>  * individuals){

    for( const auto& it : this->pop_measures ){  // for each measure

        this->pop_measures[it.first] = 0;  // cleans the old value of mean

        for(int i=0; i < individuals->size(); i++) {

            this->pop_measures[it.first] += individuals->at(i)->getMeasures()[it.first]; // sums the values of all individuals for the measure
        }

        this->pop_measures[it.first] /= (double)individuals->size(); // divides by the total

        std::cout<<"population  average "<< it.first<<" "<<this->pop_measures[it.first]<<std::endl;
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
  //  return genome1;
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
        Genome * gen = new Genome(test_genome, test_genome, test_genome);
        gen->setGeneticString(gen->build_genetic_string(gen->getGeneticString(), gs));
        gen->getGeneticString().display_list();

        // decodes the final genetic-string into a tree of components
        //std::cout << " >> decoding ... " << std::endl;
        gen->decodeGeneticString(LS, params);

        // generates robot-graphics
        //std::cout << " >> constructing ... " << std::endl;
        gen->constructor(argc, argv, this->params, 1, "testpop");

        // measures all metrics od the genome
        Measures m;
        m.setGenome(gen);
        m.measurePhenotype( this->params, 1, "testpop");

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
    std::vector<int> index_selected = std::vector<int>();

    for(int i=0; i < this->params["pop_size"]; i++) { // selects a subset of fit genomes

        int genome = this->tournament(); // selects one genome by tournament
        while(std::find(index_selected.begin(), index_selected.end(), genome) != index_selected.end()) { // makes sure that the same genome wont be selected more than once
            genome = this->tournament();
        }
        selected.push_back(this->population[genome]);
        index_selected.push_back(genome);
        //std::cout<<"selected"<<genome<<std::endl;
    }

//    for(int i=0; i < this->params["pop_size"]; i++) {
//
//        if(!(std::find(index_selected.begin(), index_selected.end(), i) != index_selected.end())){ // if genome is not on the selected list
//            delete this->population[i]; // frees memory for the not selected genomes
//            this->population[i] = NULL;
//        }
//    }

    this->population = selected; // substitutes current population for the selected subset
}


/**
 *  Saves state of the generations to file.
 */

void Evolution::saveResults(int generation){


    std::ofstream evolution_file;
    std::string path = "../../tests/evolution.txt";
    evolution_file.open(path, std::ofstream::app);

    double maximum_fitness = 0;
    double average_fitness = 0;

    // evolution_file<< generation <<" ";

    evolution_file<< generation << " " ;

    for(int i=0; i < this->getPopulation().size(); i++){


         if(this->getPopulation()[i]->getFitness() > maximum_fitness){  // finds the maximum fitness of the population

             maximum_fitness = this->getPopulation()[i]->getFitness();
         }
        average_fitness += this->getPopulation()[i]->getFitness();  //  sums all fitnesses

       // evolution_file << this->getPopulation()[i]->getId() << " "<< this->getPopulation()[i]->getFitness() << std::endl;

    }

    average_fitness /= this->getPopulation().size();  // finds the average of the fitnesses

    evolution_file  << maximum_fitness << " "  << average_fitness  << " " << std::endl;

    evolution_file.close();

}


/**
*  Evolution in the search for novelty.
**/

void Evolution::noveltySearch(int argc, char* argv[]) {


    // loads alphabet with letters and commands
    LSystem LS;
    LS.build_commands();
    LS.build_alphabet();

    this->readParams(); // read parameters for the experiment

    std::cout<<"---------------- generation 1"<<std::endl;

    this->initPopulation(LS); // initializes population
    std::vector<Genome *> * ini_pop =  new std::vector<Genome *>();
    for(int i=0; i < this->getPopulation().size(); i++){
        ini_pop->push_back(this->getPopulation()[i]);
    }

    this->developIndividuals(argc, argv, LS, 1, ini_pop, "offspringpop");  // develops genomes of the initial population
    this->measureIndividuals(1, ini_pop, "offspringpop"); // measures phenotypes of the individuals
    this->updatePop_measures_average(ini_pop);  // updates the average measures for the population
    this->evaluateIndividuals(ini_pop); // evaluates fitness of the individuals

    this->saveResults(1);

    for(int i=2; i <= params["num_generations"]; i++) { // evolves population through generations

        std::cout<<" generation "<<i<<std::endl;

        std::vector<Genome *> * offspring =  new std::vector<Genome *>();

        this->crossover(LS, offspring); // creates offspring

        this->developIndividuals(argc, argv, LS, i, offspring, "offspringpop");  // develops genomes of the generation
        this->measureIndividuals( i, offspring, "offspringpop"); // measures phenotypes of the individuals


        std::vector<Genome *> * temp_pop =  new std::vector<Genome *>();
        for(int i=0; i < this->getPopulation().size(); i++){
            temp_pop->push_back(this->getPopulation()[i]);
        }
        for(int i=0; i < offspring->size(); i++){
            temp_pop->push_back(offspring->at(i));
        }


        this->updatePop_measures_average(temp_pop);  // updates the average measures for the population

        this->evaluateIndividuals(offspring); // evaluates fitness of the individuals


        for(int i=0; i < offspring->size(); i++){  // adds new individuals to population

            this->population.push_back(offspring->at(i));
        }

        this->selection(); // selects individuals, keeping the population with a fixed size

        this->exportPop(argc, argv, LS, i, "selectedpop");

        this->saveResults(i);

    }

}


/**
*   Performs crossover among individuals in the population.
**/

void Evolution::crossover(LSystem LS, std::vector<Genome *>  * offspring){


    for(int i = 0; i < ceil(this->params["pop_size"] * this->params["offspring_size"]); i++) { // creates new individuals via crossover (size of offspring is relative to the size of population)

        int parent1 = this->tournament(); // selects parents for the crossover, according to INDEX
        int parent2 = parent1;

        while(parent2 == parent1){ // makes sure that parent2 is different from parent1
            parent2 = this->tournament();
        }

        std::string id_parent1 = this->population[parent1]->getId(); // fetches the id of the genomes
        std::string id_parent2 = this->population[parent2]->getId();

        if(parent1 == parent2 ) { // #TEST
            std::cout << " crossover with equal parents " <<parent1<<std::endl;
            exit(EXIT_FAILURE);
        }

        Genome * gen = new Genome(std::to_string(this->next_id), id_parent1, id_parent2);
        std::cout<<" crossover for genome " << this->next_id << " p1: "<<id_parent1<<" p2: "<<id_parent2<<std::endl;

        this->next_id++;

        std::map< std::string, GeneticString >  grammar = std::map< std::string, GeneticString >();

        std::random_device rd;
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<double> prob(0.0, 1.0); // distribution for probabilities

        for ( auto &it : LS.getAlphabet()) { // for each letter in the grammar of the parents

           // std::uniform_int_distribution<int> dist_type_cross(1, 3); //type 3 is too disruptive

            int  type_cross = 0;

            if(prob(generator) <= params["prop_parent"]){
                type_cross = 1;
            }else{
                type_cross = 2;
            }

            //std::cout<<" ----------- typecross "<<it.first<<" "<<type_cross<<" "<<std::endl;

            if (type_cross == 1) { // gets the genetic-string from a single parent (parent1)

                grammar.emplace(it.first, this->population[parent1]->getGrammar()[it.first]);
            }

            if (type_cross == 2) { // gets the genetic-string from a single parent (parent2)

                grammar.emplace(it.first, this->population[parent2]->getGrammar()[it.first]);
            }

//            if (type_cross == 3) { // gets a random part of the genetic-string from each parent
//
//                std::uniform_int_distribution<int> dist_pos_parent1_ini(1, this->population[parent1]->getGrammar()[it.first].count()); // distribution for parent1 initial position
//                std::uniform_int_distribution<int> dist_pos_parent2_ini(1, this->population[parent2]->getGrammar()[it.first].count()); // distribution for parent2 initial position
//
//                int pos_parent1_ini = dist_pos_parent1_ini(generator);
//                int pos_parent2_ini = dist_pos_parent2_ini(generator);
//
//                std::uniform_int_distribution<int> dist_pos_parent1_end(pos_parent1_ini, this->population[parent1]->getGrammar()[it.first].count()); // distribution for parent1 final position
//                std::uniform_int_distribution<int> dist_pos_parent2_end(pos_parent2_ini, this->population[parent2]->getGrammar()[it.first].count()); // distribution for parent2 final position
//
//                int pos_parent1_end = dist_pos_parent1_end(generator);
//                int pos_parent2_end = dist_pos_parent2_end(generator);
//
//                GeneticString gs;
//
//               // std::cout<<" ------- pos "<<pos_parent1_ini<<" "<<pos_parent1_end<<" "<<pos_parent2_ini<<" "<<pos_parent2_end <<std::endl;
//
//                gs.create_joined_list(pos_parent1_ini, pos_parent2_ini, pos_parent1_end, pos_parent2_end, this->population[parent1]->getGrammar()[it.first], this->population[parent2]->getGrammar()[it.first]);
//
//
//                grammar.emplace(it.first, gs);
//            }

            //grammar[it.first].display_list();
        }

        gen->setGrammar(grammar);

        offspring->push_back(gen); // adds new individual to the offspring

    }


    this->mutation(LS, offspring); // mutates new individuals


 }


/**
 * Performs mutation to individuals of the offspring.
 * @param LS - Lsystem structure containing the alphabet.
 */

void Evolution::mutation(LSystem LS, std::vector<Genome *> * offspring){

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_letter(1, LS.getAlphabetIndex().size()-1); // distribution for letters of the alphabet (does not include position 0, which is core-component)
    std::uniform_int_distribution<int> dist_command(1, LS.getCommands().size()-1); // distribution for the mounting commands (positions 1-3)
    std::uniform_real_distribution<double> prob(0.0, 1.0); // distribution for probabilities

    for(int i=0; i < offspring->size(); i++) {  // for each genome of the offspring


       // std::cout << "----- mut g " << offspring->at(i)->getId() << std::endl;

        for ( auto &it : offspring->at(i)->getGrammar()) { // for each letter in the grammar

            //std::cout << "letter " << it.first << std::endl;
            //std::cout << "before " <<std::endl;
            //it.second.display_list();

            //  if there is at least more than two components, and if the raffled probability is within the constrained probability
            if (it.second.count() >= 3 and prob(generator) < this->params["mutation_alter_prob"]) {

                //std::cout << " mutation remove in " << offspring->at(i)->getId() <<  std::endl;
                std::uniform_int_distribution<int> pos_d(1, it.second.count()); // distribution for position of deletion in the genetic-string
                int pos_deletion = pos_d(generator);

                if(!(it.first == "C" and pos_deletion == 0)){ // if it is the production rule of the core-component, prevents core-component from being deleted, preserving the root
                    it.second.remove(pos_deletion); // removes from random position
                }
            }

            std::vector<std::string> genetic_string_items = std::vector<std::string>();
            std::uniform_int_distribution<int> pos_i(0, it.second.count()); // distribution for position of insertion in the genetic-string

            if (prob(generator) < this->params["mutation_alter_prob"]) { // if raffled probability is within the constrained probability
                //std::cout << " mutation add command in " << offspring->at(i)->getId() <<  std::endl;
                genetic_string_items.push_back(LS.getCommands()[dist_command(generator)]); // raffles a command to add
            }
            if (prob(generator) < this->params["mutation_alter_prob"]) { // if raffled probability is within the constrained probability
                //std::cout << " mutation add letter in " << offspring->at(i)->getId() <<  std::endl;
                int test = dist_letter(generator);

                genetic_string_items.push_back(LS.getAlphabetIndex()[test]); // raffles a letter to add
            }
            if (prob(generator) < this->params["mutation_alter_prob"]) { // if raffled probability is within the constrained probability
                //std::cout << " mutation add btp " << offspring->at(i)->getId() <<  std::endl;
                genetic_string_items.push_back(LS.getCommands()[0]); // adds back-to-parent command (position 0 )
            }

            int pos_insertion = pos_i(generator);
            if(it.first == "C" and pos_insertion == 0){ // if it is the production rule of the core-component, prevents new items from being inserted at the beginning, preserving the root
                pos_insertion++;
            }

            //  (possibly) alters genetic-string (production rule) adding items (moving command, letter or back-to-parent command).
            //  in case of letter, never includes core-component, because that is the only component which should certainly be included only once
            it.second.add(pos_insertion, genetic_string_items);

            //std::cout << "after " <<std::endl;
            //it.second.display_list();
        }

    }

}


std::vector<Genome *> Evolution::getPopulation(){

    return this->population;
}

std::map<std::string, double> Evolution::getParams(){
    return params;
}