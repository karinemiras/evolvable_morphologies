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

class  Evolution{


public:
    // name of the experiment / if the experiment should be started from scratch (1) or not (0)
    explicit Evolution(std::string experiment_name, int new_experiment){
        this->experiment_name = experiment_name;
        this->new_experiment = new_experiment;
    }


    void readParams();
    void developGenome(int argc, char* argv[], Genome * gen, LSystem LS);
    void testGeneticString(int argc, char* argv[],std::string test_genome);
    void measureIndividuals(int generation, std::vector<Genome *>  * individuals, std::string dirpath);
    void evaluateIndividuals(int generation, std::vector<Genome *>  * individuals_reference, std::vector<Genome *>  * individuals_compare);
    int  tournament();
    void selection();
    std::vector<Genome *>  * getPopulation();
    std::map<std::string, double> getParams();
    int noveltySearch(int argc, char* argv[], int encodingtype);
    void exportGenerationMetrics(int generation,  int niche_coverage_generation, int niche_coverage_accumulated);
    void exportPop(int generation);
    void addToArchive( std::vector<Genome *>  * individuals, double prob_add_archive, std::string path);
    void saveParameters();
    void logsTime(std::string moment);
    void setupEvolution();
    void writesEvolutionState(int generation, int next_id);
    std::vector<std::string> readsEvolutionState();
    void loadsParams();
    void loadPopulation(int generation);
    void loadArchive();
    std::vector<int> calculateNicheCoverage();
    void createHeader();
    void updateParameter(std::string key, double value);
    void developIndividuals(int argc, char* argv[], LSystem LS, int generation, std::vector<Genome *>  * individuals, std::string path, int encodingtype);
    int loadExperiment();
    int initExperiment(int argc, char* argv[], LSystem LS, int encodingtype);
    void summaryNicheCoverage();
    void compareIndividuals(int generation);

    virtual void initPopulation(LSystem LS){};
    virtual void crossover(LSystem LS, std::vector<Genome *> * offspring){};
    virtual void mutation(LSystem LS, std::vector<Genome *> * offspring){};



protected:


    std::map<std::string, double> params =  std::map<std::string, double>(); // contains the list of parameters loaded from parameter file
    std::map< std::string, Genome * >  * archive = new std::map< std::string , Genome * > ();
    int next_id = 0; // id that will be given for the next genome to be created
    std::string experiment_name = ""; // name for the experiment
    int new_experiment; // if state of previous a experiment is being restored (1) or not (0)
    // points in a grid representing the morphological space
    std::map<std::string, std::vector<double>> morphological_grid_generation =  std::map<std::string, std::vector<double>>();
    std::map<std::string, std::vector<double>> morphological_grid_accumulated =  std::map<std::string, std::vector<double>>();
    Aux aux = Aux(this->experiment_name, this->getParams()); // contains general auxiliar methos for the experiments
    Tests tests = Tests(this->experiment_name, this->getParams()); // contains methods with tests for the system

    std::vector<Genome *>  * population = new std::vector<Genome *>(); // contains the genomes of all the individuals of the population


};


#endif //LSYSTEM_PROTO_EVOLUTION_H
