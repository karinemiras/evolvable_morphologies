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



class DecodedGeneticString{


public:


    struct node{
        std::string item;
        QGraphicsRectItem * comp;
        int id;
        struct node *left;
        struct node *front;
        struct node *right;
        struct node *back; // parent component in the tree
    };

    DecodedGeneticString::node * getRoot();

    void decode(GeneticString gs, LSystem LS);

    DecodedGeneticString(){
        root = NULL;
    }


private:

    node *root;// = new node;

};

#endif //LSYSTEM_PROTO_DECODEDGENOME_H
