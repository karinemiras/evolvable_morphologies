//
// Created by Karine Miras on 21/03/2017.
//

#ifndef LSYSTEM_PROTO_EVOLUTIONINDIRECT_H
#define LSYSTEM_PROTO_EVOLUTIONINDIRECT_H

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

class EvolutionIndirect: public Evolution{


public:


    EvolutionIndirect(std::string experiment_name,
                      int new_experiment,
                      std::string type_experiment) :
            Evolution(experiment_name,
                      new_experiment,
                      type_experiment){ }


    void initPopulation(LSystem LS);
    void crossover(LSystem LS, std::vector<Genome >  &offspring);
    void mutation(LSystem LS, std::vector<Genome> * offspring);



};


#endif //LSYSTEM_PROTO_EVOLUTIONINDIRECT_H
