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

    Genome(std::string _id){
        id = _id;
    }

    unsigned int getTo();
    GeneticString build_genetic_string(GeneticString gs, std::vector<std::string> genetic_string_items);
    void build_grammar(LSystem LS, int num_initial_comp, double add_backtoparent_prob);
    GeneticString getGeneticString();
    void setGeneticString(GeneticString _gs);
    std::vector<std::string> getAxiom();
    void generate_final_string(int  replacement_iterations, int export_genomes);
    void decodeGeneticString(LSystem LS);
    void constructor(int argc, char* argv[], int show_phenotypes, int export_phenotypes);
    void draw_component(std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *>  items,DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2);
    std::string getId();
    void exportGenome();
    void createEmbryo();
    void developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS);
    void measurePhenotype();
    void measureComponent(DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2);
    void initalizeMeasures();
    std::map< std::string, double> getMeasures();


private:

    GeneticString gs;
    DecodedGeneticString dgs;
    std::vector<std::string> axiom;
    std::map< std::string, GeneticString >  grammar;
    std::string id;
    QGraphicsScene * scene;
    std::map< std::string, double >  measures;
    std::vector<int> coor_x;
    std::vector<int> coor_y;
    std::map< std::pair<int, int>, std::string >  list_components;

};

#endif //LSYSTEM_PROTO_GENOME_H
