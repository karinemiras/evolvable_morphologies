//
// Created by Karine Miras on 21/03/2017.
//

#ifndef LSYSTEM_PROTO_EVOLUTIONDIRECT_H
#define LSYSTEM_PROTO_EVOLUTIONDIRECT_H

#include <map>
#include <string>
#include <vector>

#include "Aux.h"
#include "Evolution.h"
#include "Genome.h"
#include "Tests.h"

/**
 * Evolutionary algorithm.
 */

class EvolutionDirect: public Evolution{


public:


    EvolutionDirect(std::string experiment_name, int new_experiment) : Evolution(experiment_name, new_experiment){ }


    void initPopulation(LSystem LS);
    void crossover(LSystem LS, std::vector<Genome *> * offspring);
    void mutation(LSystem LS, std::vector<Genome *> * offspring);



};


#endif //LSYSTEM_PROTO_EVOLUTIONDIRECT_H
