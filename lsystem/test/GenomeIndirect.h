//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENOMEINDIRECT_H
#define LSYSTEM_PROTO_GENOMEINDIRECT_H

#include <map>
#include <string>
#include <vector>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsLineItem>

#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "Genome.h"
#include "LSystem.h"


/**
 * Genome of an individual with indirect encoding.
 */

class GenomeIndirect: public Genome{

public:

    explicit GenomeIndirect(std::string _id, std::string _id_parent1, std::string _id_parent2, double _fit_parent1, double _fit_parent2) : Genome( _id,  _id_parent1,  _id_parent2,  _fit_parent1, _fit_parent2){

    }

    void developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation, std::string path);

    void build_grammar(LSystem LS, std::map<std::string, double> params);
    std::vector<std::string> getAxiom();
    void generate_final_string(int  replacement_iterations, int export_genomes, int generation, std::string path);
    void createEmbryo();
    std::map< std::string, GeneticString > getGrammar();
    void setGrammar(std::map< std::string, GeneticString > grammar);




};

#endif //LSYSTEM_PROTO_GENOMEINDIRECT_H
