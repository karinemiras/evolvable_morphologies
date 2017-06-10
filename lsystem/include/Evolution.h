//
// Created by Karine Miras on 21/03/2017.
//

#ifndef LSYSTEM_PROTO_EVOLUTION_H
#define LSYSTEM_PROTO_EVOLUTION_H

#include <map>
#include <string>
#include <vector>

#include "Aux.h"
#include "Genome.h"
#include "Tests.h"

/**
 * Evolutionary algorithm.
 */

class Evolution{


public:

    Evolution(std::string experiment_name_){
        this->experiment_name = experiment_name_;
    }


    void initPopulation( LSystem LS);
    void readParams();
    void developGenome(int argc, char* argv[], Genome * gen, LSystem LS);
    void loadPopulation(int argc, char* argv[],int size_pop, std::string test_folder, LSystem LS);
    void testGeneticString(int argc, char* argv[],std::string test_genome, LSystem LS);
    void measureIndividuals(int generation, std::vector<Genome *>  * individuals, std::string dirpath);
    void evaluateIndividuals(std::vector<Genome *> * individuals);
    void compareIndividuals(std::vector<Genome *>  * individuals_reference, std::vector<Genome *>  * individuals_compare);
    int  tournament();
    void selection();
    void crossover(LSystem LS, std::vector<Genome *> * offspring);
    void mutation(LSystem LS, std::vector<Genome *> * offspring);
    std::vector<Genome *>  * getPopulation();
    std::map<std::string, double> getParams();
    void developIndividuals(int argc, char* argv[], LSystem LS, int generation, std::vector<Genome *>  * individuals, std::string path);
    void noveltySearch(int argc, char* argv[]);
    void saveResults(int generation);
    void exportPop(int argc, char* argv[], LSystem LS, int generation, std::string path);
    void addToArchive( std::vector<Genome *>  * individuals, double prob_add_archive, std::string path);
    void saveParameters();
    void logsTime(std::string moment);
    void setupEvolution();

private:

    std::vector<Genome *>  * population = new std::vector<Genome *>(); // contains the genomes of all the individuals of the population
    std::map<std::string, double> params =  std::map<std::string, double>(); // contains the list of parameters loaded from parameter file
    std::map< std::string, Genome * >  * archive = new std::map< std::string , Genome * > ();
    int next_id = 0; // id that will be given for the next genome to be created
    Tests tests = Tests();
    std::string experiment_name = "";
    Aux aux = Aux(this->experiment_name, this->getParams());;

};


#endif //LSYSTEM_PROTO_EVOLUTION_H
