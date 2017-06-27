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
    *    export_phenotypes - if exports the phenotypes to images (1) or not (0)
    *    export_genomes - if exports the genomes to files (1) or not (0)
    *    replacement_iterations - number of replacement iterations for the l-system
    *    size_component - size of each component in pixels
    *    spacing - spacing between components in pixels
    *    offspring_size - proportion of the size of the population to calculate the number of individuals of the offsfpring
    *    num_generations - number of generations of the evolution
    *    mutation_alter_prob - probability of adding/removing items (letters/commands) to the genetic-string in the mutation
    *    max_comps - maximum number of components allowed per phenotype
    *    prop_parent - proportion from each parent in the crossover (1 leads to no crossover)
    *    prob_add_archive - probability of adding any genome to the archive
    *    k_neighbors - number of neighbords to compare for fitness
    *    logs_to_screen - if exports the logs to the screen (1) or not (0)
    *    logs_to_file - if exports logs to a file (1) or not (0)
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
    } else {
        this->aux.logs("Unable to open parameters file.");
    }

}

/*
 * Change value of parameter for an experiment.
 * */
void Evolution::updateParameter(std::string key, double value){

    this->params[key] = value;
}


/**
 * Loads parameters from saved state.
 **/
void Evolution::loadsParams(){

    std::string line;
    std::ifstream myfile ("../../experiments/"+this->experiment_name+"/configuration.txt");

    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {

            std::vector<std::string> tokens;
            // parameters label and value separated by space
            boost::split( tokens, line, boost::is_any_of(" ") );

            // first item is the label, second is the value
            this->params[tokens[0]] = std::stod(tokens[1]);
        }
        myfile.close();
    } else {
        this->aux.logs("Unable to open parameters state.");
    }

}



/**
 * Tries to add individuals to an archive for innovation.
 * @param individuals - list of genomes
 * @param prob_add_archive - probability of adding some genome to the archive of novelty
 * @param path - directory to save backups of the genome of the archive
 **/
void Evolution::addToArchive( std::vector<Genome *>  * individuals, double prob_add_archive, std::string path){

    std::random_device rd;
     std::default_random_engine generator(rd());
    // distribution for 0-1 probabilities
     std::uniform_real_distribution<double> prob(0.0, 1.0);

    for(int i=0; i < individuals->size(); i++) {

        // if raffled probability is within the constrained probability
        if (prob(generator) < prob_add_archive) {

            Genome * gen_arc = nullptr;
            gen_arc = new Genome(*individuals->at(i)); // copies object of the genome to archive

            this->archive->emplace(individuals->at(i)->getId(),gen_arc);

            if(this->params["export_genomes"] == 1){
                individuals->at(i)->exportGenome(path+"/archive");
            }
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

    for(int i=0; i < this->population->size(); i++) { // for each genome in the population

        this->population->at(i)->constructor(argc, argv, this->params, path+std::to_string(generation)); // generates phenotype
    }
}



/**
 * Measures all the individuals of the population for several metrics.
 *  @param argc - command line parameter
 *  @param argv[] - command line parameter
 *  @param individuals - array with genomes
 *  @param dirpath - name of the output directory
 **/
void Evolution::measureIndividuals(int generation, std::vector<Genome *>  * individuals, std::string dirpath){

    for(int i=0; i < individuals->size(); i++) {  // for each genome of the population

        Measures * m = new Measures(this->experiment_name, this->params);
        m->setGenome(individuals->at(i));
        m->measurePhenotype(this->params, dirpath+std::to_string(generation)); // measures phenotype
        delete m;
    }
}




/**
 * Creates files of results containing headers.
 */

void Evolution::createHeader(){

    std::ofstream file;

    std::string path = "../../experiments/"+this->experiment_name+"/history.txt";
    file.open(path, std::ofstream::app);
    file << "generation" << " idgenome" << " fitgenome" << " idparent1" << " fitparent1" << " idparent2" << " fitparent2" << std::endl;
    file.close();

    path = "../../experiments/"+this->experiment_name+"/evolution.txt";
    file.open(path, std::ofstream::app);
    file << "generation" << " maxfitness" << " meanfitness" << " nichecoverage" << std::endl;
    file.close();

}



/**
 * Calculates the fitness for the individuals: average euclidean distance from the nearest neighbors
 * @param generation - number of the generation for the evolution
 * @param individuals_reference - individuals of current-population (parents+offspring)
 * @param individuals_compare - individuals representing current-population+archive
 */

void Evolution::evaluateIndividuals(int generation, std::vector<Genome *>  * individuals_reference, std::vector<Genome *>  * individuals_compare){


    std::ofstream history_file;
    std::string path = "../../experiments/"+this->experiment_name+"/history.txt";
    history_file.open(path, std::ofstream::app);


    // matrix with all individuals
    // columns: number of metrics / lines: number of genomes
    arma::mat compare(individuals_compare->at(0)->getMeasures().size() , individuals_compare->size());

    for(int i=0; i < individuals_compare->size(); i++) {

        int m = 0;
        for( const auto& it : individuals_compare->at(i)->getMeasures()){

            compare(m, i) = it.second;
            m ++;
        }

    }


    for(int i=0; i < individuals_reference->size(); i++) {

        // matrix with individuals which will be compared to the others
        // columns: number of metrics / single line: genome
        arma::mat reference(individuals_reference->at(0)->getMeasures().size(), 1);

        int m = 0;
        for( const auto& it : individuals_reference->at(i)->getMeasures()){

            reference(m, 0) = it.second;
            m ++;
        }

        NeighborSearch<NearestNeighborSort, EuclideanDistance> nn(compare);
        arma::Mat<size_t> neighbors;
        arma::mat distances;

        // search for each individual, the nearest neighbors (+1 because it includes itself)
        nn.Search(reference, this->params["k_neighbors"]+1, neighbors, distances);

        double fitness = 0;
        for (size_t j = 0; j < neighbors.n_elem; ++j) {

             fitness +=  distances[j];
             this->aux.logs("nearest neighbor  "+std::to_string(j)+" for genome " +individuals_reference->at(i)->getId()+ " has distance " + std::to_string(distances[j]));
         }

        // averages the nearest neighboards
        fitness = fitness/this->params["k_neighbors"];
        individuals_reference->at(i)->updateFitness(fitness);


        this->aux.logs("fitness genome " + individuals_reference->at(i)->getId()+ " : "+ std::to_string(individuals_reference->at(i)->getFitness()));

        history_file << std::to_string(generation)<<" "     // generation
                     << individuals_reference->at(i)->getId()<<" "   // idgenome
                     << std::to_string(individuals_reference->at(i)->getFitness())<<" "  // fitness genome
                     << individuals_reference->at(i)->getId_parent1()<<" "  // id of parent1
                     << individuals_reference->at(i)->getFit_parent1()<<" "  // fitness of parent1
                     << individuals_reference->at(i)->getId_parent2()<<" " // id of parent2
                     << individuals_reference->at(i)->getFit_parent2() // fitness of parent2
                     << std::endl;

    }

    history_file.close();

}




/**
 * Selects two random genomes and compares their fitness, choosing the winner.
 * @return - the index of the winner genome
 */

int Evolution::tournament(){

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> dist_1(0, (int) this->population->size()-1);

    int genome1 =  dist_1(generator); // random genome 1
    int genome2 =  dist_1(generator); // random genome 2

    // return the genome with higher fitness
    if (this->population->at(genome1)->getFitness() >  this->population->at(genome2)->getFitness()){
        return genome1;
    }else{
        return genome2;
    }

}



/**
 * Loads population of genomes from files, from previous experiment.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param size_pop - size of the population to be loaded.
 * @param test_folder - the path with folder/file from where to read the population files.
 * @param LS - Lsystem structure containing the alphabet.
 **/

//void Evolution::loadPopulation(int argc, char* argv[],int size_pop, std::string test_folder, LSystem LS){
//
//    std::string line;
//    std::stringstream convert;
//
//    for (int i=1; i<=size_pop; i++) { // for each genome file in the specified folder, reads its contents
//
//        convert << i; // if of genome
//        std::ifstream myfile("../../experiments/" + test_folder + "/genome_" + convert.str() + ".txt");
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

//};


/**
 * Test a genome made from a had-designed genetic-string.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param test_genome - the path with folder/file from where to read the genome to be tested.
 * @param LS - Lsystem structure containing the alphabet.
 */

void Evolution::testGeneticString(int argc, char* argv[],std::string test_genome) {

    this->readParams();

    LSystem LS;
    LS.build_mounting_commands();
    LS.build_moving_commands();
    LS.build_alphabet();

    std::string line;
    std::ifstream myfile("../../experiments/fixed/" + test_genome+ ".txt");
    if (myfile.is_open()) {
        getline(myfile, line);
        std::vector<std::string> tokens;
        // items of the genetic-string separated by space
        boost::split(tokens, line, boost::is_any_of(" "));
        std::vector<std::string> gs;

        // adds each item in the genetic-string in the array of items
        for(int j=0; j<tokens.size(); j++){
            if(tokens[j] != "") { gs.push_back(tokens[j]); }
        }

        // creates new genome with id equal 1, using the just read genetic-string
        Genome * gen = new Genome(test_genome, test_genome, test_genome, -1, -1);
        gen->setGeneticString(gen->build_genetic_string(gen->getGeneticString(), gs));
        gen->getGeneticString().display_list();

        // decodes the final genetic-string into a tree of components
        //std::cout << " >> decoding ... " << std::endl;
        gen->decodeGeneticString(LS, params);

        // generates robot-graphics
        //std::cout << " >> constructing ... " << std::endl;
        gen->constructor(argc, argv, this->params, "fixed");

        // measures all metrics od the genome
        Measures m(this->experiment_name, this->params);
        m.setGenome(gen);
        m.measurePhenotype( this->params, "fixed");

        myfile.close();
    }else{
        this->aux.logs("Cant open genome.");
    }
}

/**
*  Selection of genomes in a population.
**/

void Evolution::selection() {

    std::vector<Genome *> * selected = new std::vector<Genome *>();
    std::vector<int> index_selected = std::vector<int>();

    // selects genomes, maintaining population size
    for(int i=0; i < this->params["pop_size"]; i++) {

        int genome = this->tournament(); // selects one genome by tournament

        // makes sure that the same genome wont be selected more than once
        while(std::find(index_selected.begin(), index_selected.end(), genome) != index_selected.end()) {
            genome = this->tournament();
        }
        selected->push_back(this->population->at(genome));
        index_selected.push_back(genome);
    }

    for(int i=0; i < this->params["pop_size"]; i++) {

        // if genome is not on the selected list
        if(!(std::find(index_selected.begin(), index_selected.end(), i) != index_selected.end())){
            delete this->population->at(i); // frees memory for the not selected genomes
            this->population->at(i) = NULL;
        }
    }

    this->population = selected; // substitutes current population for the selected subset

    // # TEST: Tests if population size remains correct.
    this->tests.testPopsize(this->population, (int) this->params["pop_size"]);
}


/**
 *  Saves state of the generations to file.
 */

void Evolution::exportGenerationMetrics(int generation, int niche_coverage){


    std::ofstream evolution_file;
    std::string path = "../../experiments/"+this->experiment_name+"/evolution.txt";
    evolution_file.open(path, std::ofstream::app);

    double maximum_fitness = 0;
    double average_fitness = 0;

    evolution_file<< generation << " " ;

    for(int i=0; i < this->getPopulation()->size(); i++){


        if(this->getPopulation()->at(i)->getFitness() > maximum_fitness){  // finds the maximum fitness of the population

            maximum_fitness = this->getPopulation()->at(i)->getFitness();
        }
        average_fitness += this->getPopulation()->at(i)->getFitness();  //  sums all fitnesses

    }

    average_fitness /= this->getPopulation()->size();  // finds the average of the fitnesses

    evolution_file  << maximum_fitness << " "  << average_fitness  << " " << niche_coverage << " " << std::endl;

    evolution_file.close();

}


void Evolution::setupEvolution(){

    // read default parameters for the experiment
    this->readParams();

    Aux aux = Aux(this->experiment_name, this->getParams());
    this->aux = Aux(this->experiment_name, this->getParams());

    // cleans old files and creates folders for the experiment
    aux.removeFolder(this->experiment_name);

    aux.createFolder(this->experiment_name);
    aux.createFolder(this->experiment_name+"/archive");
    aux.createFolder(this->experiment_name+"/offspringpop1");

    // logs parameters configuration
    this->saveParameters();

}




/**
*  Develops genomes of the population: 1- grows genetic-string of the genome according to grammar, 2- decodes it, 3- constructs the phenotype
*  @param argc - default argument
*  @param argv[] - default argument
*  @param LS - Lsystem structure containing the alphabet
*  @param individuals - array with genomes
**/
void Evolution::developIndividuals(int argc, char* argv[], LSystem LS, int generation, std::vector<Genome *> * individuals, std::string path, int encodingtype){

    if (encodingtype == 1) {
        // for each genome in the array
        for (int i = 0; i < individuals->size(); i++) {

            // develops genome
            individuals->at(i)->developGenomeIndirect(argc, argv, this->params, LS, generation, path);

        }
    }

    if (encodingtype == 0) {
        // for each genome in the array
        for (int i = 0; i < individuals->size(); i++) {

            // develops genome
            individuals->at(i)->developGenomeDirect(argc, argv, this->params, LS, generation, path);

        }
    }
}



/**
*  Evolution in the search for novelty.
**/

int Evolution::noveltySearch(int argc, char* argv[], int encodingtype) {

    int niche_coverage = 0, max_niche_coverage = 0;


    // loads alphabet with letters and commands
    LSystem LS;
    LS.build_mounting_commands();
    LS.build_moving_commands();
    LS.build_alphabet();


    int gi = NULL; // initial generation

    // if experiment is set to start from the beginning
    if(this->new_experiment == 1) {

        // reads parameters for new experiment and creates directories
        this->setupEvolution();

        this->logsTime("start");
        this->createHeader();

        gi = 1; // start evolution from first generation

        this->aux.logs("---------------- generation "+std::to_string(gi)+" ----------------");

        // initializes population
        this->initPopulation(LS);

        // saves the number of the last generation created/evaluated
        this->writesGeneration(gi);


    // if experiment is set to continue from previous experiment
    }else{


        this->logsTime("start");
        this->createHeader();

        // loads state of parameters from previous experiment
        this->loadsParams();
        this->aux = Aux(this->experiment_name, this->getParams());

        gi = this->readsGeneration(); // generation from which to start evolution

        this->aux.logs("---------------- generation "+std::to_string(gi)+" ----------------");

        // loads population and archive
       // this->loadPopulation(); // #FIX: finish it, loading pop/archive/state etc !!!!!!!

    }

    // develops genomes of the initial population
    this->developIndividuals(argc, argv, LS, gi, this->population, this->experiment_name+"/offspringpop",  encodingtype);

    // measures phenotypes of the individuals
    this->measureIndividuals(gi, this->population, "/offspringpop");

    // updates the average measures for the population
    this->evaluateIndividuals(gi, this->population, this->population);

    // (possibly) adds genome to archive
    this->addToArchive(this->population, this->params["prob_add_archive"], this->experiment_name);

    // calculates quality state of the search for the generation
    niche_coverage = this->calculateNicheCoverage();

    // saves metrics of evolution to file
    this->exportGenerationMetrics(gi, niche_coverage);



    // evolves population through out generations
    for(int g = gi+1; g <= params["num_generations"]; g++) {

        this->aux.logs("---------------- generation " + std::to_string(g) + " ----------------");

        std::vector<Genome *> * offspring =  new std::vector<Genome *>();

        // creates offspring
        this->crossover(LS, offspring);

        this->aux.createFolder(this->experiment_name+"/offspringpop"+std::to_string(g));
        this->aux.createFolder(this->experiment_name+"/selectedpop"+std::to_string(g));

        // develops genomes of the offspring
        this->developIndividuals(argc, argv, LS, g, offspring, this->experiment_name+"/offspringpop",  encodingtype);
        // measures phenotypes of the offspring
        this->measureIndividuals( g, offspring, "/offspringpop");


        // BEGINNING: auxiliar pointers //

            std::vector<Genome *> * temp_pop_reference =  new std::vector<Genome *>();
            std::vector<Genome *> * temp_pop_compare = new std::vector<Genome *>();

            for(int j=0; j < this->getPopulation()->size(); j++){

                temp_pop_reference->push_back(this->getPopulation()->at(j));
                temp_pop_compare->push_back(this->getPopulation()->at(j));

            }

            for(int j=0; j < offspring->size(); j++){

                temp_pop_reference->push_back(offspring->at(j));
                temp_pop_compare->push_back(offspring->at(j));
            }

            for ( auto &it : *this->archive){

                temp_pop_compare->push_back(this->archive->at(it.first));
            }


        // END: auxiliar pointers //


        // evaluates population (parents+offspring)
        this->evaluateIndividuals(g, temp_pop_reference, temp_pop_compare);

        // (possibly) adds genomes to archive
        this->addToArchive(offspring, this->params["prob_add_archive"], this->experiment_name);
        // logs arquive
        for( const auto& it : *this->archive ){
            this->aux.logs(" archive " + it.first);
        }

        // adds new individuals to population
        for(int j=0; j < offspring->size(); j++){
            this->population->push_back(offspring->at(j));
        }

        // selects individuals, keeping the population with a fixed size
        this->selection();

        // calculates quality state of the search for the generation
        niche_coverage = this->calculateNicheCoverage();

        // saves metrics of evolution to file
        this->exportGenerationMetrics(g, niche_coverage);

        // saves phenotypes of the selected population to a separated folder (only for visualization issues)
        this->exportPop(argc, argv, LS, g, this->experiment_name+"/selectedpop");

        // saves the number of the last generation created/evaluated
        this->writesGeneration(g);

    }

    this->logsTime("end");

    // updates the maximum niche coverage found so far
    if(niche_coverage > max_niche_coverage) max_niche_coverage = niche_coverage;


    return max_niche_coverage;

}




std::vector<Genome *> * Evolution::getPopulation(){
    return this->population;
}

std::map<std::string, double> Evolution::getParams(){
    return params;
}


/*
 * Exports the parameters of the experiment.
 **/
void Evolution::saveParameters() {

    std::ofstream param_file;
    std::string path = "../../experiments/"+this->experiment_name+ "/configuration.txt";
    param_file.open(path);

    // writes each parameter to a different line in a the file
    for (auto &it : this->getParams()) {

        param_file << it.first << " " << it.second;
        param_file << std::endl;
    }
    param_file.close();
}


/*
 * Logs time.
 **/
void Evolution::logsTime(std::string moment){

    time_t sta = time(0);
    char* dtsta = ctime(&sta);
    this->aux.logs("experiment " + moment + ": " +dtsta);

}

/*
 * Logs the generation from which the recovered evolution should start from.
 * */
void Evolution::writesGeneration(int generation) {

    std::ofstream logs_file;
    std::string path = "../../experiments/"+ this->experiment_name +"/recovergeneration.txt";
    logs_file.open(path);
    logs_file << generation;
    logs_file.close();
}

/*
 * Reads number of the generation from which the recovered evolution should start from.
 * */
int Evolution::readsGeneration() {

    std::string line;
    std::ifstream myfile("../../experiments/"+ this->experiment_name +"/recovergeneration.txt");
    if (myfile.is_open()) {
        getline(myfile, line);
        return std::stoi(line);
    } else {
        this->aux.logs("Unable to open generation file.");
    }
    myfile.close();

}


/*
 * Calculates the quality metric for the novelty search: niche coverage
 * */
int Evolution::calculateNicheCoverage() {

    // total number of points in the grid given the dimentions and spacing
    //int total_points = std::pow(this->population->at(0)->getMeasures().size(),this->params["grid_bins"]);

    this->morphological_grid =  std::map<std::string, std::vector<double>>();

    for(int i = 0; i < this->population->size(); i++ ) {

        std::string key_point = "";
        double distance = 0;

        // for each measure (dimension)
        for(const auto& it : this->population->at(i)->getMeasures()){

            // for each bin
            for(int b=1; b <= this->params["grid_bins"]; b++) {

                // if value is zero, sets into the first bin
                if (it.second == 0 and b == 1) {

                    key_point += std::to_string(b) + "|";
                    distance += -1 * (it.second - (b / this->params["grid_bins"]));


                }
                // otherwise, sets value for measure into the correct bin
                if (it.second > ((b - 1) / this->params["grid_bins"]) and
                    it.second <= (b / this->params["grid_bins"])) {

                    key_point += std::to_string(b) + "|";
                    distance += -1 * (it.second - (b / this->params["grid_bins"]));
                }

            }
        }


        // if point already exists in the array, adds an individual and the difference between them
        if(this->morphological_grid.count(key_point)>0) {

            this->morphological_grid[key_point].push_back(distance);

        // if point does not exist in the array yet, , adds new point with its first individual and the difference between them
        }else {
            std::vector<double> individual; individual.push_back(distance);
            this->morphological_grid[key_point] = individual;
        }
    }


    for(const auto& it : this->morphological_grid) {

        this->aux.logs("morphological_grid point "+it.first+" contains "+std::to_string(it.second.size())+" individuals");

    }

    return (int)this->morphological_grid.size();


}

