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
using namespace mlpack::neighbor; // NeighborSearch and NeaxrestNeighborSort
using namespace mlpack::metric; // EuclideanDistance

#include "../Genome/RobotRepresentation.h"
#include "../Genome/Mutator.h"
#include "../Genome/EvolverConfiguration.h"

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
    std::ifstream myfile ("../configuration.txt");

    /*   pop_size - size of the population of genomes
    *    offspring_prop - proportion of the population size to dcalculate size of offspring
    *    num_initial_comp - number of initial (random) components in the production rules of the grammar
    *    show_phenotypes - flag to show the phenotype graphic
    *    export_phenotypes - if exports the phenotypes to images (1) or not (0)
    *    export_genomes - if exports the genomes to files (1) or not (0)
    *    replacement_iterations - number of replacement iterations for the l-system
    *    size_component - size of each component in pixels
    *    spacing - spacing between components in pixels
    *    num_generations - number of generations of the evolution
    *    mutation_prob - probability of adding/removing/swaping items (letters/commands) to the genetic-string in the mutation
    *    max_comps - maximum number of components allowed per phenotype
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
    std::ifstream myfile ("../experiments/"+this->experiment_name+"/configuration.txt");

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

            this->archive->emplace(individuals->at(i)->getId(), gen_arc);

//            if(this->params["export_genomes"] == 1){
//                individuals->at(i)->exportGenome(path+"/archive");
//            }
        }
    }
}


/* Finds out in which generation the genome was generated.
 * @param idgenome - id of the genome for which to verify generation.
 * */
int Evolution::getGeneration_genome(std::string idgenome){

    int generation_genome = 0;
    int offspring_size = this->params["pop_size"] * this->params["offspring_prop"];

    // generation of the genome can be found by its id, considering the size of the population and the offspring
    if(this->params["offspring_prop"] == 1)
        generation_genome = (int)trunc( std::stoi(idgenome)
                                        /this->params["pop_size"])+1;
    else
        generation_genome = (int)trunc( (std::stoi(idgenome)-offspring_size)
                                        /offspring_size)+1;

    if (generation_genome == 0) generation_genome = 1;

    return generation_genome;

}



/**
*  Copies the phenotypes of the selection population to a separate folder.
*  @param generation - number of generation in evolution
**/
void Evolution::exportPop(int generation){

    std::ofstream measures_file_general;
    std::string path = "../experiments/"+this->experiment_name+"/measures2.txt";
    measures_file_general.open(path, std::ofstream::app);


    for(int i=0; i < this->population->size(); i++) { // for each genome in the population


        // finds number of generation to which the genome belongs to
        int generation_genome = this->getGeneration_genome(this->population->at(i)->getId());

        std::string filename = "/body_"+this->population->at(i)->getId()
                               +"_p1_"+this->population->at(i)->getId_parent1()+"_p2_"+this->population->at(i)->getId_parent2()+".png";

        std::string pathfrom =  "../experiments/"
                                + this->experiment_name + "/offspringpop"+ std::to_string(generation_genome);

        std::string pathto =  "../experiments/"
                              + this->experiment_name + "/selectedpop"+ std::to_string(generation);

        // copies phenotype file from offspring folder to selected population folder
        system(("exec cp "+pathfrom+filename+" "+pathto+filename).c_str());

        // copies values of metrics to file of selected population
        std::string line;
        std::ifstream measures("../experiments/"+this->experiment_name+
                                       "/offspringpop"+std::to_string(generation_genome)
                               +"/measures"+this->population->at(i)->getId()+".txt");

        while (getline (measures, line) ) {

            std::vector<std::string> tokens;
            boost::split( tokens, line, boost::is_any_of(":") );

            measures_file_general << std::to_string(generation)<< " "
                                  <<this->population->at(i)->getId()<< " "
                                  << tokens[0] <<" " << tokens[1] <<std::endl;
        }
    }

    measures_file_general.close();

}


/*
 * Compare phenotype of the individual with its parent's.
 * */
double Evolution::compareIndividual(Measures * m, std::string idgenome){


    int generation_genome = this->getGeneration_genome(idgenome);

    std::string line;
    std::ifstream measures("../experiments/"+this->experiment_name+
                           "/offspringpop"+std::to_string(generation_genome)+"/measures"+idgenome+".txt");

    double dif = 0;
    while (getline (measures, line) ) {

        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(":"));

        dif +=  std::pow(m->getGenome()->getMeasures()[tokens[0]] - std::stod(tokens[1]) , 2);
    }
    dif =  roundf(std::sqrt(dif)*100)/100;

    return dif;

}


/*
 * Compare phenotype of the parents.
 * */
double Evolution::compareParents(std::string idparent1, std::string idparent2){


    int generation_genome_parent1 = this->getGeneration_genome(idparent1);
    std::string line;
    std::ifstream measures("../experiments/"+this->experiment_name+
                           "/offspringpop"+std::to_string(generation_genome_parent1)+"/measures"+idparent1+".txt");

    int generation_genome_parent2 = this->getGeneration_genome(idparent2);
    std::string line2;
    std::ifstream measures2("../experiments/"+this->experiment_name+
                           "/offspringpop"+std::to_string(generation_genome_parent2)+"/measures"+idparent2+".txt");

    double dif = 0;
    while (getline (measures, line) ) {

        getline (measures2, line2);

        std::vector<std::string> tokens, tokens2;
        boost::split(tokens, line, boost::is_any_of(":"));
        boost::split(tokens2, line2, boost::is_any_of(":"));

        dif +=  std::pow(std::stod(tokens[1]) - std::stod(tokens2[1]) , 2);
    }
    dif =  roundf(std::sqrt(dif)*100)/100;

    return dif;

}

/**
 * Measures all the individuals of the population for several metrics.
 *  @param argc - command line parameter
 *  @param argv[] - command line parameter
 *  @param individuals - array with genomes
 *  @param dirpath - name of the output directory
 **/
void Evolution::measureIndividuals(int generation, std::vector<Genome *>  * individuals, std::string dirpath){

    std::ofstream differences_file;
    std::string path = "../experiments/"+this->experiment_name+"/differences.txt";
    differences_file.open(path, std::ofstream::app);


    // for each genome of the population
    for(int i=0; i < individuals->size(); i++) {

        Measures * m = new Measures(this->experiment_name, this->params);
        m->setGenome(individuals->at(i));
        // measures phenotype
        m->measurePhenotype(this->params, dirpath, generation);


        // compares measures between individuals
        if(individuals->at(i)->getId_parent1()!="N"){

            double dif = this->compareIndividual(m, individuals->at(i)->getId_parent1());
            differences_file << individuals->at(i)->getId() << " " << dif;

            dif = this->compareIndividual(m, individuals->at(i)->getId_parent2());
            differences_file <<  " " << dif;

            dif = this->compareParents(individuals->at(i)->getId_parent1(), individuals->at(i)->getId_parent2());
            differences_file <<  " " << dif<<std::endl;

        }

        delete m;
    }

    differences_file.close();
}



/*
 *
 * */




/**
 * Creates files of results containing headers.
 */

void Evolution::createHeader(){

    std::ofstream file;

    std::string path = "../experiments/"+this->experiment_name+"/history.txt";
    file.open(path);
    file << "generation" << " idgenome" << " fitgenome" << " idparent1" << " fitparent1" << " idparent2" << " fitparent2" << " meandif" << std::endl;
    file.close();

    path = "../experiments/"+this->experiment_name+"/evolution.txt";
    file.open(path);
    file << "generation" << " maxfitness" << " meanfitness" << " nichecoverage_generation"  << " nichecoverage_accumulated" << std::endl;
    file.close();

    path = "../experiments/"+this->experiment_name+"/measures.txt";
    file.open(path);
    file << "generation"<<" idgenome" << " branching"<<" connectivity1"<<" effective_joints"<<" joints_per_limb"<<" length_ratio"<<" sparseness"<<" symmetry"<<" total_components"<<std::endl;
    file.close();

    path = "../experiments/"+this->experiment_name+"/measures2.txt";
    file.open(path);
    file << "generation"<<" genome" << " measures"<<" value"<<std::endl;
    file.close();

    path = "../experiments/"+this->experiment_name+"/nichecoverage_distances.txt";
    file.open(path);
    file << "generation"<<" average" << " stddev "<<std::endl;
    file.close();

    path = "../experiments/"+this->experiment_name+"/differences.txt";
    file.open(path);
    file << "idgenome"  << " difference_parent1"  << " difference_parent2"  << " difference_parents" << std::endl;
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
    std::string path = "../experiments/"+this->experiment_name+"/history.txt";
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


        history_file << std::to_string(generation)<<" "     // generation
                     << individuals_reference->at(i)->getId()<<" "   // idgenome
                     << individuals_reference->at(i)->getFitness()<<" "  // fitness genome
                     << individuals_reference->at(i)->getId_parent1()<<" "  // id of parent1
                     << individuals_reference->at(i)->getFit_parent1()<<" "  // fitness of parent1
                     << individuals_reference->at(i)->getId_parent2()<<" " // id of parent2
                     << individuals_reference->at(i)->getFit_parent2()<<" " // fitness of parent2
                     <<   (individuals_reference->at(i)->getFitness()-individuals_reference->at(i)->getFit_parent1())
                        + (individuals_reference->at(i)->getFitness()-individuals_reference->at(i)->getFit_parent2())
                         / (float)2// mean dif fitness from parents
                     << std::endl;

    }

    history_file.close();

}



/**
 * Compares average distance among points inthe accumulaetd grid.
 */

void Evolution::compareIndividuals(int generation){

    std::ofstream file;
    std::string path = "../experiments/"+this->experiment_name+"/nichecoverage_distances.txt";
    file.open(path, std::ofstream::app);


    // fetches points with its dimensions
    std::vector<std::vector<double>> points;
    for( const auto& it : this->morphological_grid_accumulated) {

        std::vector<double> dimensions;

        std::vector<std::string> tokens;
        boost::split(tokens, it.first, boost::is_any_of("|"));
        std::vector<std::string> tokens2(tokens.begin(), tokens.begin()+tokens.size()-1);

        for(int i=0; i < tokens2.size(); i++){
            dimensions.push_back(std::stod(tokens2[i])/this->params["grid_bins"]);
        }
        points.push_back(dimensions);
    }


    double avgdistance = 0;
    std::vector<double> avgdistance_points;
    double stddev_distance = 0;

    // for each point
    for(int i=0; i < points.size(); i++){

        double avgdistance_point = 0;
        // compare to every other point
        for(int j=0; j < points.size(); j++){

            // for each dimension
            double distance = 0;
            for(int d=0; d < this->population->at(0)->getMeasures().size(); d++) {

                distance += std::pow(points[i][d] - points[j][d], 2);
            }
            // euclidean distance
            distance = std::sqrt(distance);

            avgdistance_point += distance;
        }
        // average distance from the point to all others
        avgdistance_point /= (points.size()-1);

        avgdistance_points.push_back(avgdistance_point);
        avgdistance += avgdistance_point;
    }

    // average distance of all points
    avgdistance /= points.size();

    for(int i=0; i < avgdistance_points.size(); i++){
        stddev_distance += std::pow(avgdistance - avgdistance_points[i], 2);
    }
    stddev_distance = std::sqrt(stddev_distance);


    file<<generation<<" "<<avgdistance<<" "<<stddev_distance<<std::endl;


    file.close();

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
    if (this->population->at(genome1)->getFitness() > this->population->at(genome2)->getFitness()){

        return genome1;
    }else{
        return genome2;
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

void Evolution::exportGenerationMetrics(int generation, int niche_coverage_generation, int niche_coverage_accumulated){


    std::ofstream evolution_file;
    std::string path = "../experiments/"+this->experiment_name+"/evolution.txt";
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

    evolution_file  << maximum_fitness << " "  << average_fitness  << " " << niche_coverage_generation << " " << niche_coverage_accumulated << " " << std::endl;

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
            individuals->at(i)->developGenomeDirect(argc, argv, this->params, generation, path);

        }
    }

}



/**
 * Initializes the population with new genomes.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void Evolution::initPopulation(LSystem LS){ // default arguments and Lsystem

    //creates new genomes according to population size
    for(int i=1; i <= this->params["pop_size"]; i++) {

        // initial genomes receive personal ids, but the ids of the parents are none
        Genome * gen = new Genome(std::to_string(this->next_id), "N", "N", -1, -1);


        std::string v1 = "../experiments/"+this->experiment_name+"/offspringpop1/"+std::to_string(this->next_id);
        std::string v3 = "../Test/evolConf-full.txt";
        std::ofstream file;
        file.open(v1+".txt");
        file<<"0 CoreComponent E0 0 GREEN";
        file.close();
        boost::shared_ptr<robogen::RobotRepresentation> robot1(new robogen::RobotRepresentation());
        robot1->init(std::string(v1)+".txt", "P0");

        boost::shared_ptr<robogen::EvolverConfiguration> evo(new robogen::EvolverConfiguration);
        evo->init(std::string(v3));
        boost::random::mt19937 rng(i);

        robogen::Mutator newr(evo, rng);
        newr.growBodyRandomly(robot1, v1);

        this->population->push_back(gen);  // adds genome to the population

        // increments id that will be used for any next genome created
        this->next_id++;
    }

}


/**
 *  Starts new experiment.
 **/
int Evolution::initExperiment(int argc, char* argv[], LSystem LS, int encodingtype){

    // reads parameters for new experiment and creates directories
    this->setupEvolution();

    this->logsTime("start");
    this->createHeader();

    int gi = 1; // start evolution from first generation

    this->aux.logs("---------------- generation "+std::to_string(gi)+" ----------------");

    // initializes population
    this->initPopulation(LS);

    // develops genomes of the initial population
    this->developIndividuals(argc, argv, LS, gi, this->population, this->experiment_name+"/offspringpop",  encodingtype);

    // measures phenotypes of the individuals
    this->measureIndividuals(gi, this->population, "/offspringpop");

    // updates the average measures for the population
    this->evaluateIndividuals(gi, this->population, this->population);

    // (possibly) adds genome to archive
    this->addToArchive(this->population, this->params["prob_add_archive"], this->experiment_name);




    return gi;
}

/**
*  Evolution in the search for novelty.
**/

int Evolution::noveltySearch(int argc, char* argv[], int encodingtype) {


    int niche_coverage_generation = 0, niche_coverage_accumulated = 0;


    // loads alphabet with letters and commands
    LSystem LS;
    LS.build_mounting_commands();
    LS.build_moving_commands();
    LS.build_alphabet();


    int gi = NULL; // initial generation

    // if experiment is set to start from the beginning
    if(this->new_experiment == 1) {


        gi = this->initExperiment(argc, argv, LS, encodingtype);

        // calculates quality state of the search for the generation
        niche_coverage_generation = this->calculateNicheCoverage()[0];
        niche_coverage_accumulated = this->calculateNicheCoverage()[1];

        // saves metrics of evolution to file
        this->exportGenerationMetrics(gi, niche_coverage_generation, niche_coverage_accumulated);

    // if experiment is set to continue from previous experiment
    }else{

       //gi = this->loadExperiment();

    }



//    // evolves population through out generations
//    for(int g = gi+1; g <= params["num_generations"]; g++) {
//
//        this->aux.logs("---------------- generation " + std::to_string(g) + " ----------------");
//
//        std::vector<Genome *> * offspring =  new std::vector<Genome *>();
//
//        // creates offspring
//        this->crossover(LS, offspring);
//
//        this->aux.createFolder(this->experiment_name+"/offspringpop"+std::to_string(g));
//        this->aux.createFolder(this->experiment_name+"/selectedpop"+std::to_string(g));
//
//        // develops genomes of the offspring
//        this->developIndividuals(argc, argv, LS, g, offspring, this->experiment_name+"/offspringpop",  encodingtype);
//        // measures phenotypes of the offspring
//        this->measureIndividuals(g, offspring, "/offspringpop");
//
//
//        // BEGINNING: auxiliar pointers //
//
//            std::vector<Genome *> * temp_pop_reference =  new std::vector<Genome *>();
//            std::vector<Genome *> * temp_pop_compare = new std::vector<Genome *>();
//
//            for(int j=0; j < this->getPopulation()->size(); j++){
//
//                temp_pop_reference->push_back(this->getPopulation()->at(j));
//                temp_pop_compare->push_back(this->getPopulation()->at(j));
//
//            }
//
//            for(int j=0; j < offspring->size(); j++){
//
//                temp_pop_reference->push_back(offspring->at(j));
//                temp_pop_compare->push_back(offspring->at(j));
//            }
//
//            for ( auto &it : *this->archive){
//
//                temp_pop_compare->push_back(this->archive->at(it.first));
//            }
//
//
//        // END: auxiliar pointers //
//
//
//        // evaluates population (parents+offspring)
//        this->evaluateIndividuals(g, temp_pop_reference, temp_pop_compare);
//
//        // (possibly) adds genomes to archive
//        this->addToArchive(offspring, this->params["prob_add_archive"], this->experiment_name);
//        // logs arquive
//        for( const auto& it : *this->archive ){
//            this->aux.logs(" archive " + it.first);
//        }
//
//        // adds new individuals to population
//        for(int j=0; j < offspring->size(); j++){
//            this->population->push_back(offspring->at(j));
//        }
//
//        // selects individuals, keeping the population with a fixed size
//        this->selection();
//
//        // calculates quality state of the search for the generation
//        niche_coverage_generation  = this->calculateNicheCoverage()[0];
//        niche_coverage_accumulated  = this->calculateNicheCoverage()[1];
//
//        // saves metrics of evolution to file
//        this->exportGenerationMetrics(g, niche_coverage_generation, niche_coverage_accumulated);
//
//        // saves phenotypes of the selected population to a separated folder (only for visualization issues)
//        this->exportPop(g);
//
//
//        //every 10 generations, compares distances among points
//        if(g%10==0) {
//            this->compareIndividuals(g);
//        }
//
//
//        // saves the number of the last generation created/evaluated
//        this->writesEvolutionState(g, this->next_id);
//
//
//    }
//
//    this->summaryNicheCoverage();


    this->logsTime("end");

    return niche_coverage_accumulated;

}


void Evolution::summaryNicheCoverage(){

    std::ofstream file;

    std::string path = "../experiments/"+this->experiment_name+"/morphological_grid_summary.txt";
    file.open(path);
    file << "point count"<< std::endl;

    for(const auto& it : this->morphological_grid_accumulated) {

        file << it.first+" "<<it.second.size()<< std::endl;
    }
    file.close();

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
    std::string path = "../experiments/"+this->experiment_name+ "/configuration.txt";
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
void Evolution::writesEvolutionState(int generation, int next_id) {

    std::ofstream logs_file;
    std::string path = "../experiments/"+ this->experiment_name +"/evolutionstate.txt";
    logs_file.open(path);
    logs_file << generation <<" "<< next_id;
    logs_file.close();
}

/*
 * Reads number of the generation from which the recovered evolution should start from.
 * */
std::vector<std::string> Evolution::readsEvolutionState() {

    std::string line;
    std::ifstream myfile("../experiments/"+ this->experiment_name +"/evolutionstate.txt");
    if (myfile.is_open()) {

        getline(myfile, line);
        std::vector<std::string> tokens;
        // parameters label and value separated by space
        boost::split( tokens, line, boost::is_any_of(" ") );

        return tokens;

    } else {
        this->aux.logs("Unable to open evolutionstate file.");
    }
    myfile.close();

}


/*
 * Calculates the quality metric for the novelty search: niche coverage
 * */
std::vector<int> Evolution::calculateNicheCoverage() {

    // total number of points in the grid given the dimentions and spacing
    //int total_points = std::pow(this->params["grid_bins"],this->population->at(0)->getMeasures().size()

    this->morphological_grid_generation =  std::map<std::string, std::vector<double>>();

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
        if(this->morphological_grid_generation.count(key_point)>0) {

            this->morphological_grid_generation[key_point].push_back(distance); // add map with key=id value=distance ?

            // if point does not exist in the array yet, , adds new point with its first individual and the difference between them
        }else {
            std::vector<double> individual; individual.push_back(distance);
            this->morphological_grid_generation[key_point] = individual;
        }



        // if point already exists in the array, adds an individual and the difference between them
        if(this->morphological_grid_accumulated.count(key_point)>0) {

            this->morphological_grid_accumulated[key_point].push_back(distance); // add map with key=id value=distance ?

            // if point does not exist in the array yet, , adds new point with its first individual and the difference between them
        }else {
            std::vector<double> individual; individual.push_back(distance);
            this->morphological_grid_accumulated[key_point] = individual;
        }


    }


    // logs state of the grid
    std::ofstream myfile;
    std::string path = "../experiments/"+ this->experiment_name +"/morphological_grid_accumulated.txt";
    myfile.open(path);
    for(const auto& it : this->morphological_grid_accumulated) {

        myfile << it.first+"-";
        for(int i=0; i<it.second.size(); i++){
            myfile << it.second[i]<<" ";
        }
        myfile << std::endl;
    }
    myfile.close();


    std::vector<int> morphological_grids;
    morphological_grids.push_back((int)this->morphological_grid_generation.size());  // pos 0
    morphological_grids.push_back((int)this->morphological_grid_accumulated.size()); // pos 1

    return morphological_grids;


}

