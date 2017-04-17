//
// Created by Karine Miras on 21/03/2017.
//

#ifndef LSYSTEM_PROTO_EVOLUTION_H
#define LSYSTEM_PROTO_EVOLUTION_H

#include <map>
#include <string>
#include <vector>

#include "Genome.h"

/**
 * Evolutionary algorithm.
 */

class Evolution{


    public:


        void initPopulation(int argc, char* argv[], LSystem LS);
        void readParams();
        void developGenome(int argc, char* argv[], Genome * gen, LSystem LS);
        void loadPopulation(int argc, char* argv[],int size_pop, std::string test_folder, LSystem LS);
        void testGeneticString(int argc, char* argv[],std::string test_genome, LSystem LS);
        void measurePopulation();

    private:

        std::vector<Genome *>  population = std::vector<Genome *>(); // contains the genomes of all the individuals of the population
        std::map<std::string, double> params =  std::map<std::string, double>(); // contains the list of parameters loaded from parameter file



};


#endif //LSYSTEM_PROTO_EVOLUTION_H
