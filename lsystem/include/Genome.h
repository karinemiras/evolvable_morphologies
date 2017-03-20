//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENOME_H
#define LSYSTEM_PROTO_GENOME_H

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
#include "LSystem.h"

/**
 *  Genome of an individual.
 */
class Genome{

public:

    Genome(std::string _id,std::vector<std::string> _axiom){
        axiom = _axiom;
        id = _id;
        validity = "valid";
    }

    unsigned int getTo();
    GeneticString build_genetic_string(GeneticString gs, std::vector<std::string> genetic_string_items);
    void build_grammar(LSystem LS);
    GeneticString getGeneticString();
    void setGeneticString(GeneticString _gs);
    std::vector<std::string> getAxiom();
    void generate_final_string();
    void setValidity(std::string validity);
    std::string getValidity();
    void decodeGeneticString(LSystem LS);
    void constructor(int argc, char* argv[]);
    void draw_component(std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *>  items,DecodedGeneticString::node * c1, DecodedGeneticString::node * c2);

private:

    GeneticString gs;
    DecodedGeneticString dgs;
    std::vector<std::string> axiom;
    std::map< std::string, GeneticString >  grammar;
    std::string id;
    std::string validity;



};

#endif //LSYSTEM_PROTO_GENOME_H
