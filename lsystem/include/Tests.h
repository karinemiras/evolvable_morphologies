//
// Created by Karine Miras on 19/05/2017.
//

#ifndef LSYSTEM_PROTO_TESTS_H
#define LSYSTEM_PROTO_TESTS_H

#include <string>

#include "Genome.h"



class Tests{

public:
    void testMeasures(std::string id_genome, std::map< std::string, double>);
    void testParents(int parent1, int parent2);
    void testPopsize(std::vector<Genome *> * population,  int pop_size);

};

#endif //LSYSTEM_PROTO_TESTS_H
