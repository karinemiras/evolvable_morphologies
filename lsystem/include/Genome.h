//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENOME_H
#define LSYSTEM_PROTO_GENOME_H

#include <map>
#include <string>
#include <vector>

#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "LSystem.h"

/**
 *  Genome of an individual.
 */
class Genome{

public:

    Genome(std::vector<std::string> _axiom){
        axiom = _axiom;
    }

    unsigned int getTo();
    GeneticString build_genetic_string(GeneticString gs, std::vector<std::string> genetic_string_items);
    void build_grammar(LSystem LS);
    GeneticString getGeneticString();
    void setGeneticString(GeneticString _gs);
    std::vector<std::string> getAxiom();
    void generate_final_string();
    void decodeGeneticString(LSystem LS);


private:

    GeneticString gs;
    DecodedGeneticString dgs;
    std::vector<std::string> axiom;
    std::map< std::string, GeneticString >  grammar;



};

#endif //LSYSTEM_PROTO_GENOME_H
