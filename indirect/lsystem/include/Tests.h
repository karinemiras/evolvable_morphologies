//
// Created by Karine Miras on 19/05/2017.
//

#ifndef LSYSTEM_PROTO_TESTS_H
#define LSYSTEM_PROTO_TESTS_H

#include <string>

#include "Aux.h"
#include "Genome.h"



class Tests{

    public:


        Tests(std::string experiment_name, std::map<std::string, double> params){
            this->params = params;
            this->experiment_name = experiment_name;
        }

        void testMeasures(std::string id_genome, std::map< std::string, double>);
        void testParents(int parent1, int parent2);
        void testPopsize(std::vector<Genome> population,  int pop_size);

    void setParams(std::map<std::string, double> params);

    std::string experiment_name;
    std::map<std::string, double> params;
    Aux aux = Aux(this->experiment_name, this->params);
};

#endif //LSYSTEM_PROTO_TESTS_H
