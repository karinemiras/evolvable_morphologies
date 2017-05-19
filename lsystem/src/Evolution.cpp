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

    //creates new genomes according to population size
    for(int i=1; i <= this->params["pop_size"]; i++) {

        // initial genomes receive personal ids, but the ids of the parents are none
        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N");

        // creates genetic-strings for the production rules of the letters in the grammar (initial random rules)
        gen->build_grammar(LS, (int) this->params["num_initial_comp"], this->params["add_backtoparent_prob"]);

        this->population.push_back(gen);  // adds genome to the population

        // increments id that will be used for any next genome created
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

    // for each genome in the array
    for(int i=0; i < individuals->size(); i++) {

        // develops genome
        individuals->at(i)->developGenome(argc, argv, this->params, LS,  generation, path);

    }
}


void Evolution::addToArchive( std::vector<Genome *>  * individuals, double prob_add_archive){

    std::random_device rd;
     std::default_random_engine generator(rd());
    // distribution for 0-1 probabilities
     std::uniform_real_distribution<double> prob(0.0, 1.0);

    for(int i=0; i < individuals->size(); i++) {

        // if raffled probability is within the constrained probability
        if (prob(generator) < prob_add_archive) {

            this->archive.emplace(individuals->at(i)->getId(), individuals->at(i));
        }
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

    for(int i=0; i < population.size(); i++) { // for each genome in the population

        population[i]->constructor(argc, argv, this->params,  generation, path); // generates phenotype
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
        m->measurePhenotype(this->params, generation, dirpath); // measures phenotype
    }
}



/**
 * Calculates the fitness of the population of genomes.
 */

void Evolution::evaluateIndividuals(std::vector<Genome *> * individuals){

    // for each genome of the population
    for(int i=0; i < individuals->size(); i++) {

        // calculates its fitness
       // individuals->at(i)->calculateFitness(this->pop_measures);
        std::cout<<"fitness genome "<<individuals->at(i)->getId()<<" : "<<individuals->at(i)->getFitness()<<std::endl;
    }
}


/**
 * Calculates the average of the measures among all genomes.
 */

void Evolution::compareIndividuals(std::vector<Genome *>  * individuals){


    for(int i=0; i < individuals->size(); i++) {  // for each reference-genome

        //compare reference-genome with the rest of the genomes in the POPULATION
        for(int j=0; j < individuals->size(); j++) {

            // if compared-genome is not the reference-genome itself and is also not in the archive
            if(i != j){

                // if comparison with compared-genome has not been calculated in the past
                if(individuals->at(i)->getGenomeDistance().count(std::to_string(j)) == 0 ) {

                    double distances = 0;  // cleans the old value of mean
                    // for each measure
                    for (auto &it : individuals->at(i)->getMeasures()) {

                        // sums the values of all individuals for the measure
                        distances += sqrt(pow(individuals->at(i)->getMeasures()[it.first] -
                                              individuals->at(j)->getMeasures()[it.first], 2));
                    }
                    // divides by the total of measures
                    distances /= (double) individuals->at(i)->getMeasures().size();

                    std::cout << " dist g " << individuals->at(j)->getId() << " " << distances << "" << std::endl;


                    std::pair<int, double> status_distance = std::make_pair(1, distances);
                    individuals->at(i)->setGenomeDistance(individuals->at(j)->getId(), status_distance);

                }else{

                    // updates status to current, keeping the same value for distance
                    individuals->at(i)->setGenomeDistanceStatus(std::to_string(j));
                }
            }
        }




        std::cout<<" compare genome " << i << "" <<std::endl;
        for( const auto& it : individuals->at(i)->getGenomeDistance() ){

            std::cout<<" value " << it.first  << " " << it.second.first  << " "<< it.second.second  << " "<<std::endl;
        }
    }
}




/**
 * Selects two random genomes and compares their fitness, choosing the winner.
 * @return - the index of the winner genome
 */

int Evolution::tournament(){

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> dist_1(0, (int) this->population.size()-1);

    int genome1 =  dist_1(generator); // random genome 1
    int genome2 =  dist_1(generator); // random genome 2

    // return the genome with higher fitness
    if (this->population[genome1]->getFitness() > this->population[genome2]->getFitness()){
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
//
//    std::string line;
//    std::stringstream convert;
//
//    for (int i=1; i<=size_pop; i++) { // for each genome file in the specified folder, reads its contents
//
//        convert << i; // if of genome
//        std::ifstream myfile("../../tests/" + test_folder + "/genome_" + convert.str() + ".txt");
//        if (myfile.is_open()) {
//            getline(myfile, line);
//            std::vector<std::string> tokens;
//            boost::split(tokens, line, boost::is_any_of(" ")); // items of the genetic-string separated by space
//
//            for(int j=0; j<tokens.size(); j++){
//                // build genetic string vector!
//            }
//
//            myfile.close();
//        }else{
//            std::cout<<"Cant open genome "<<i;
//        }
//    }

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

    // selects genomes, maintaining population size
    for(int i=0; i < this->params["pop_size"]; i++) {

        int genome = this->tournament(); // selects one genome by tournament

        // makes sure that the same genome wont be selected more than once
        while(std::find(index_selected.begin(), index_selected.end(), genome) != index_selected.end()) {
            genome = this->tournament();
        }
        selected.push_back(this->population[genome]);
        index_selected.push_back(genome);
    }

//    for(int i=0; i < this->params["pop_size"]; i++) {
//
//        if(!(std::find(index_selected.begin(), index_selected.end(), i) != index_selected.end())){ // if genome is not on the selected list
//            delete this->population[i]; // frees memory for the not selected genomes
//            this->population[i] = NULL;
//        }
//    }

    this->population = selected; // substitutes current population for the selected subset

    // # TEST: Tests if population size remains correct.
    this->tests.testPopsize(this->population, (int) this->params["pop_size"]);
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

    // initializes population
    this->initPopulation(LS);
    std::vector<Genome *> * ini_pop =  new std::vector<Genome *>();
    for(int i=0; i < this->getPopulation().size(); i++){

        ini_pop->push_back(this->getPopulation()[i]);
    }

    // develops genomes of the initial population
    this->developIndividuals(argc, argv, LS, 1, ini_pop, "offspringpop");
    // measures phenotypes of the individuals
    this->measureIndividuals(1, ini_pop, "offspringpop");
    // updates the average measures for the population
    this->compareIndividuals(ini_pop);
    // evaluates fitness of the individuals
    this->evaluateIndividuals(ini_pop);
    // saves metrics of evolution to file
    this->saveResults(1);
    // (possibly) adds genome to archive
    this->addToArchive(ini_pop, this->params["prob_add_archive"]);



    for( const auto& it : this->archive ){

        std::cout<<" archive "<<it.first<<" "<<it.second<<std::endl;

    }

    // evolves population through generations
    for(int i=2; i <= params["num_generations"]; i++) {

        std::cout<<" generation "<<i<<std::endl;

        std::vector<Genome *> * offspring =  new std::vector<Genome *>();

        // creates offspring
        this->crossover(LS, offspring);

        // develops genomes of the offspring
        this->developIndividuals(argc, argv, LS, i, offspring, "offspringpop");
        // measures phenotypes of the offspring
        this->measureIndividuals( i, offspring, "offspringpop");

        // joins parents with offspring as new population
        std::vector<Genome *> * temp_pop =  new std::vector<Genome *>();
        for(int i=0; i < this->getPopulation().size(); i++){

            temp_pop->push_back(this->getPopulation()[i]);
        }
        for(int i=0; i < offspring->size(); i++){

            temp_pop->push_back(offspring->at(i));
        }

        // updates the average measures for the population
        this->compareIndividuals(temp_pop);

        std::vector<Genome *> * aux_archive =  new std::vector<Genome *>();
        for ( auto &it : this->archive){

            aux_archive->push_back(this->archive[it.first]);
        }
        this->compareIndividuals(aux_archive);

        // evaluates fitness of the offspring
        this->evaluateIndividuals(offspring);
        // (possibly) adds genome to archive
        this->addToArchive(offspring, this->params["prob_add_archive"]);

        // adds new individuals to population
        for(int i=0; i < offspring->size(); i++){

            this->population.push_back(offspring->at(i));
        }

        // selects individuals, keeping the population with a fixed size
        this->selection();

        // saves phenotypes of the selected population to a separated folder
        this->exportPop(argc, argv, LS, i, "selectedpop");

        // saves metrics of evolution to file
        this->saveResults(i);



        for( const auto& it : this->archive ){

            std::cout<<" archive "<<it.first<<" "<<it.second<<std::endl;

        }


    }



}


/**
*   Performs crossover among individuals in the population.
**/

void Evolution::crossover(LSystem LS, std::vector<Genome *>  * offspring){


    // creates new individuals via crossover (size of offspring is relative to the size of population)
    for(int i = 0; i < ceil(this->params["pop_size"] * this->params["offspring_size"]); i++) {

        // selects parents for the crossover, according to INDEX
        int parent1 = this->tournament();
        int parent2 = parent1;

        while(parent2 == parent1){ // makes sure that parent2 is different from parent1
            parent2 = this->tournament();
        }

        // fetches the id of the genomes
        std::string id_parent1 = this->population[parent1]->getId();
        std::string id_parent2 = this->population[parent2]->getId();

        // #TEST: Tests if selected parents are different.
        this->tests.testParents(parent1, parent2);

        // creates new offspring genome
        Genome * gen = new Genome(std::to_string(this->next_id), id_parent1, id_parent2);
        std::cout<<" crossover for genome " << this->next_id << " p1: "<<id_parent1<<" p2: "<<id_parent2<<std::endl;

        this->next_id++;

        std::map< std::string, GeneticString >  grammar = std::map< std::string, GeneticString >();

        std::random_device rd;
        std::default_random_engine generator(rd());

        std::uniform_real_distribution<double> prob(0.0, 1.0); // distribution for probabilities

        for ( auto &it : LS.getAlphabet()) { // for each letter in the grammar

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

void Evolution::mutation(LSystem LS, std::vector<Genome *> * offspring){

    std::random_device rd;
    std::default_random_engine generator(rd());

    // distribution for letters of the alphabet (does not include position 0, which is core-component, as it should be present only once)
    std::uniform_int_distribution<int> dist_letter(1, (int) LS.getAlphabetIndex().size()-1);

    // distribution for the mounting commands (positions 1-3, does not include position 0, which is back-to-parent command)
    std::uniform_int_distribution<int> dist_command(1, (int) LS.getCommands().size()-1);

    // distribution for 0-1 probabilities
    std::uniform_real_distribution<double> prob(0.0, 1.0);


    for(int i=0; i < offspring->size(); i++) {  // for each genome of the offspring


        for ( auto &it : offspring->at(i)->getGrammar()) { // for each letter in the grammar

            //  if there is at least more than two components, and if the raffled probability is within the constrained probability
            if (it.second.count() >= 3 and prob(generator) < this->params["mutation_alter_prob"]) {

                //std::cout << "----- mut g " << offspring->at(i)->getId() << " remove in " << offspring->at(i)->getId() <<  std::endl;

                // distribution for position of deletion in the genetic-string
                std::uniform_int_distribution<int> pos_d(1, it.second.count());
                int pos_deletion = pos_d(generator);

                // if it is the production rule of the core-component, prevents core-component from being deleted, preserving the root
                if(!(it.first == "C" and pos_deletion == 0)){
                    it.second.remove(pos_deletion); // removes item from chosen position
                }
            }

            std::vector<std::string> genetic_string_items = std::vector<std::string>();
            // distribution for position of insertion in the genetic-string
            std::uniform_int_distribution<int> pos_i(0, it.second.count());

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_alter_prob"]) {

                //std::cout << "----- mut g " << offspring->at(i)->getId() << " add command in " << offspring->at(i)->getId() <<  std::endl;
                // raffles a command to add
                genetic_string_items.push_back(LS.getCommands()[dist_command(generator)]);
            }

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_alter_prob"]) {

                //std::cout << "----- mut g " << offspring->at(i)->getId() << " add letter in " << offspring->at(i)->getId() <<  std::endl;
                // raffles a letter to add
                genetic_string_items.push_back(LS.getAlphabetIndex()[dist_letter(generator)]);
            }

            // if raffled probability is within the constrained probability
            if (prob(generator) < this->params["mutation_alter_prob"]) {

                //std::cout << "----- mut g " << offspring->at(i)->getId() << "add btp " << offspring->at(i)->getId() <<  std::endl;
                // adds back-to-parent command
                genetic_string_items.push_back(LS.getCommands()[0]);
            }

            // if it is the production rule of the core-component, prevents new items from being inserted at the beginning, preserving the root
            int pos_insertion = pos_i(generator);
            if(it.first == "C" and pos_insertion == 0){
                pos_insertion++;
            }

            //  (possibly) alters genetic-string (production rule) adding items (moving command, letter or back-to-parent command).
            it.second.add(pos_insertion, genetic_string_items);

        }

    }

}


std::vector<Genome *> Evolution::getPopulation(){
    return this->population;
}

std::map<std::string, double> Evolution::getParams(){
    return params;
}