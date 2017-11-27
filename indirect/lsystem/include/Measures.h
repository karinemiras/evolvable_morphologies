//
// Created by Karine Miras on 12/04/2017.
//

#ifndef LSYSTEM_PROTO_MEASURES_H
#define LSYSTEM_PROTO_MEASURES_H

#include <map>
#include <string>
#include <vector>

#include "DecodedGeneticString.h"
#include "Genome.h"
#include "LSystem.h"

/**
 *  Measures of a genome's morphology.
 */

class Measures{

public:

    Measures(std::string experiment_name, std::map<std::string, double> params){
        this->experiment_name = experiment_name;
        this->params = params;
    }

    void initalizeMeasures();
    void measurePhenotype(std::map<std::string, double> params,
                          std::string dirpath, int generation);
    void measureComponent(std::string reference,
                          std::string direction,
                          DecodedGeneticString::Vertex * c1,
                          DecodedGeneticString::Vertex * c2,
                          std::map<std::string, double> params);
    std::map< std::string, double> getMeasures();
    void setGenome(Genome &gen);
    Genome * getGenome();
    std::pair<int, int> find_points(DecodedGeneticString::Vertex * c1,
                                    DecodedGeneticString::Vertex * c2,
                                    int x,
                                    int y);


private:

    Genome * gen = nullptr; // pointer to the genome to be measured
    // points outlining the polygon formed by the morphology through the components
    std::map< std::pair<int, int> , double> points = std::map< std::pair<int, int> , double>();
    std::string experiment_name = "";
    std::map<std::string, double> params =  std::map<std::string, double>();

};

#endif //LSYSTEM_PROTO_MEASURES_H
