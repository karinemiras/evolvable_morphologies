//
// Created by Karine Miras on 08/03/2017.
//

#ifndef LSYSTEM_PROTO_DECODEDGENOME_H
#define LSYSTEM_PROTO_DECODEDGENOME_H

#include <string>

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsRectItem>

#include "GeneticString.h"
#include "LSystem.h"

/**
 * Structure of the items composing the second developmental stage a genome. The structure is a graph of which the nodes represent components of the body.
 */

class DecodedGeneticString{


public:


    struct Vertex{
        std::string item;
        int id;
        int x;
        int y;
        Vertex *left;
        Vertex *front;
        Vertex *right;
        Vertex *back; // parent component in the tree
        Vertex() { left=front=right=back=NULL; x=y=0;}
    };

    DecodedGeneticString::Vertex * getRoot();

    void decode(GeneticString * gs, LSystem LS, std::map<std::string, double> params);

    DecodedGeneticString(){
        root = NULL;
    }


private:

    Vertex *root;  // item of the graph of components

};

#endif //LSYSTEM_PROTO_DECODEDGENOME_H
