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
 * Genome of an individual.
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
        void generate_final_string(int  replacement_iterations, int export_genomes, int generation);
        void decodeGeneticString(LSystem LS);
        void constructor(int argc, char* argv[], std::map<std::string, double> params, int generation);
        void draw_component(std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *>  items,DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2, std::map<std::string, double> params);
        std::string getId();
        void exportGenome(int generation);
        void createEmbryo();
        void developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation);
        DecodedGeneticString getDgs();
        std::map< std::string, double > getMeasures();
        std::map< std::pair<int, int>, std::string >  getList_components();
        void updateMeasure(std::string key, double value);
        void calculateFitness(std::map< std::string, double > pop_measures);
        double getFitness();
        std::map< std::string, GeneticString > getGrammar();
        void setGrammar(std::map< std::string, GeneticString > grammar);
        void removeMeasure(std::string key);

    private:

        std::string id; // id identifying the genome
        double fitness = 0; // fitness of the genome
        GeneticString gs; // main genetic-string of the genome
        DecodedGeneticString dgs; // graph that logically represents the connections among the components forming the body
        QGraphicsScene * scene; // scene holding the phenotype
        std::vector<std::string> axiom = std::vector<std::string>(); // letter(s) of the alphabet to build the initial developmental state of the genetic-string
        std::map< std::string, GeneticString >  grammar = std::map< std::string, GeneticString >(); // genetic-strings of production rules for each letter of the alphabet
        std::map< std::string, double >  measures = std::map< std::string, double >(); // list of body metrics about the genome
        std::map< std::pair<int, int>, std::string >  list_components = std::map< std::pair<int, int>, std::string >(); // list of all components of the body, keys are coordinates, value is a letter

};

#endif //LSYSTEM_PROTO_GENOME_H
