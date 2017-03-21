//
// Created by Karine Miras on 21/03/2017.
//

#ifndef LSYSTEM_PROTO_EVOLUTION_H
#define LSYSTEM_PROTO_EVOLUTION_H

#include <map>
#include <string>
#include <vector>


#include "Genome.h"


class Evolution{

public:

    void initPopulation(int argc, char* argv[], LSystem LS);
    void readParams();

private:

    std::vector<Genome> population;
    std::map<std::string, int> params;


};


#endif //LSYSTEM_PROTO_EVOLUTION_H
