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
#include "Measures.h"
#include "Tests.h"

//from tol.spec import get_body_spec, get_brain_spec

/**
 * Evolutionary algorithms.
 */
class  Evolution{


public:

    explicit Evolution(std::string experiment_name,
                       int new_experiment,
                       std::string type_experiment){

        this->experiment_name = experiment_name;
        this->new_experiment = new_experiment;
        this->type_experiment = type_experiment;

        this->measures_names.push_back("branching");
        this->measures_names.push_back("connectivity1");
        this->measures_names.push_back("connectivity2");
        this->measures_names.push_back("coverage");
        this->measures_names.push_back("effective_joints");
        this->measures_names.push_back("length_ratio");
        this->measures_names.push_back("sensors");
        this->measures_names.push_back("symmetry");
        this->measures_names.push_back("total_components");

        for(int i=0;i<this->measures_names.size(); i++) {
            this->morphological_measures_accumulated[this->measures_names[i]] = std::vector<double>();
        }

    }

    void saveHistory(int generation, Genome individual);

    void readParams();
    void testGeneticString(int argc,
                           char* argv[],
                           std::string test_genome);
    void measureIndividuals(int generation,
                            std::vector<Genome>  &individuals,
                            std::string dirpath);
    void evaluateNS(int generation,
                             std::vector<Genome>  &offspring);

    void evaluateLocomotion(int generation,
                     std::vector<Genome >  &individuals);

    int  tournament();
    void selection();
    std::vector<Genome>  getPopulation();
    std::map<std::string, double> getParams();
    double runExperiment(int argc, char* argv[]);
    void exportGenerationMetrics(int generation,
                                 std::vector<int> metrics);
    void exportPop(int generation);
    void addToArchive( std::vector<Genome>  individuals,
                       double prob_add_archive,
                       std::string path);
    void saveParameters();
    void logsTime(std::string moment);
    void setupEvolution();
    void writesEvolutionState(int generation, int next_id);
    std::vector<std::string> readsEvolutionState();
    void loadsParams();
    void loadPopulation(int generation);
    void loadArchive();
    std::vector<int> calculateNicheCoverage(std::vector<Genome>  individuals);
    void createHeader();
    void updateParameter(std::string key, double value);
    void developIndividuals(int argc, char* argv[],
                            LSystem LS,
                            int generation,
                            std::vector<Genome>  &individuals,
                            std::string path);
    int loadExperiment();
    int initExperiment(int argc, char* argv[],
                       LSystem LS);
    void summaryNicheCoverage();
    void compareIndividuals(int generation);
    int getGeneration_genome(std::string idgenome);
    double compareIndividual(Measures m,
                             std::string idgenome);
    double compareParents(std::string idparent1,
                          std::string idparent2);

    void cleanMemory(std::vector< int > index_selected);
    void cleanVertex(DecodedGeneticString::Vertex * v);

    virtual void initPopulation(LSystem LS){};
    virtual void crossover(LSystem LS, std::vector<Genome> &offspring){};
    virtual void mutation(LSystem LS, std::vector<Genome> &offspring){};



protected:

    std::vector<std::string> measures_names =
            std::vector<std::string>();

    std::map<std::string, double> params =
            std::map<std::string, double>(); // contains the list of parameters loaded from parameter file

    std::map< std::string, Genome >  archive =
             std::map< std::string , Genome > ();

    int next_id = 0; // id that will be given for the next genome to be created

    std::string experiment_name = ""; // name for the experiment

    int new_experiment=1; // if state of previous a experiment is being
    // restored (1) or not (0)

    std::string type_experiment = ""; // name of the type: novelty, locomotion etc

    // points in a grid representing the morphological space
    std::map<std::string, std::vector<double>>
            morphological_grid_generation =
            std::map<std::string, std::vector<double>>();

    std::map<std::string, std::vector<std::string>>
            morphological_grid_accumulated =
            std::map<std::string, std::vector<std::string>>();

    std::map<std::string, std::vector<double>>
            morphological_measures_accumulated =
            std::map<std::string, std::vector<double>>();

    Aux aux = Aux(this->experiment_name, this->getParams()); // contains general auxiliar methos for the experiments
    Tests tests = Tests(this->experiment_name, this->getParams()); // contains methods with tests for the system

    std::vector<Genome>  population =  std::vector<Genome>();
    // contains the genomes of all the individuals of the population


};


#endif //LSYSTEM_PROTO_EVOLUTION_H
