//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENOME_H
#define LSYSTEM_PROTO_GENOME_H

#include <map>
#include <string>
#include <vector>

#include "GeneticString.h"
#include "LSystem.h"

/*
 *  Genome of an individual.
 */
class Genome{


public:

    Genome(std::vector<std::string> _axiom){
        axiom = _axiom;
    }

    GeneticString build_genetic_string(GeneticString gs, std::vector<std::string> genetic_string_items);
    void build_grammar(LSystem LS);
    GeneticString getGeneticString();
    void setGeneticString(GeneticString _gs);
    std::vector<std::string> getAxiom();
    std::map< std::string, GeneticString > getGrammar();
    void generate_final_string();

private:

    GeneticString gs;
    std::vector<std::string> axiom;
    std::map< std::string, GeneticString >  grammar;
};

#endif //LSYSTEM_PROTO_GENOME_H
