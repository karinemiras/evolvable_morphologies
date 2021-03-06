//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENOME_H
#define LSYSTEM_PROTO_GENOME_H

#include <map>
#include <string>
#include <vector>

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsRectItem>
#include <QtGui/QGraphicsLineItem>

#include "DecodedGeneticString.h"
#include "LSystem.h"


/**
 * Genome of an individual.
 */

class Genome{

public:

    Genome(std::string _id, std::string _id_parent1, std::string _id_parent2, double _fit_parent1, double _fit_parent2){
        id = _id;
        id_parent1 = _id_parent1;
        id_parent2 = _id_parent2;
        fit_parent1 = _fit_parent1;
        fit_parent2 = _fit_parent2;
    }



    unsigned int getTo();
    std::vector<std::string> getAxiom();
    void decodeGeneticString(std::string path, std::map<std::string, double> params);
    void constructor(int argc, char* argv[], std::map<std::string, double> params, std::string path);
    void draw_component(std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *>  items,DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2, std::map<std::string, double> params);
    std::string getId();
    std::string getId_parent1();
    std::string getId_parent2();
    double getFit_parent1();
    double getFit_parent2();
    void developGenomeDirect(int argc, char* argv[], std::map<std::string, double> params, int generation, std::string path);
    DecodedGeneticString getDgs();
    std::map< std::string, double > getMeasures();
    std::map< std::pair<int, int>, std::string >  getList_components();
    void updateMeasure(std::string key, double value);
    void calculateFitness(int k_neighboards);
    double getFitness();
    void removeMeasure(std::string key);
    void updateFitness(double fitness);
    void setDecodedGeneticString(DecodedGeneticString dgs);



protected:

    std::string id; // id identifying the genome
    std::string id_parent1; // id of parent1 of genome
    std::string id_parent2; // id of parent2 of genome
    double fit_parent1;  // fit of parent1 of genome
    double fit_parent2;  // fit of parent2 of genome
    double fitness = 0; // fitness of the genome
    DecodedGeneticString dgs; // graph that logically represents the connections among the components forming the body
    QGraphicsScene * scene; // scene holding the phenotype
    // letter(s) of the alphabet to build the initial developmental state of the genetic-string
    std::vector<std::string> axiom = std::vector<std::string>();
    // genetic-strings of production rules for each letter of the alphabet
    std::map< std::string, double >  measures = std::map< std::string, double >(); // list of body metrics about the genome
    // list of all components of the body, keys are coordinates, value is a letter
    std::map< std::pair<int, int>, std::string >  list_components = std::map< std::pair<int, int>, std::string >();


};

#endif //LSYSTEM_PROTO_GENOME_H