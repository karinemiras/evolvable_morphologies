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

        void initalizeMeasures();
        void measurePhenotype(int argc, char* argv[], std::map<std::string, double> params, int generation);
        void measureComponent(std::string reference, std::string direction, DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2, std::map<std::string, double> params);
        std::map< std::string, double> getMeasures();
        void setGenome(Genome * gen);
        Genome * getGenome();
        std::pair<int, int> find_points(DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2,  int x, int y);


    private:

        Genome * gen; // pointer to the genome to be measured
        std::map< std::pair<int, int> , double> points = std::map< std::pair<int, int> , double>(); // points outlining the polygon formed by the morphology through the components


};

#endif //LSYSTEM_PROTO_MEASURES_H
