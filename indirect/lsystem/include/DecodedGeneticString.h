//
// Created by Karine Miras on 08/03/2017.
//

#ifndef LSYSTEM_PROTO_DECODEDGENOME_H
#define LSYSTEM_PROTO_DECODEDGENOME_H

#include <string>

#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QApplication>
#include <QtGui/QGraphicsRectItem>

#include "GeneticString.h"
#include "LSystem.h"

/**
 * Structure of the items composing the second (late) developmental stage a
 * genome. This structure represents the phenotype.
 * The structure is made of two parts:
 *  - a graph whose nodes represent the morpgology of the body.
 *  - a graph whose the nodes represent a neural network.
 */

class DecodedGeneticString{


public:

    // body structure
    struct Vertex
    {
        std::string item; // a letter or command
        int id;
        int x; // coordinates
        int y;
        Vertex *left; // pointers to body-part attachements around component
        Vertex *front;
        Vertex *right;
        Vertex *back;
        std::string sensor_left; // attachment of sensors to component
        std::string sensor_front;
        std::string sensor_right;
        std::string sensor_back;

        Vertex()
        {
            left=front=right=back=NULL;
            sensor_left=sensor_front=sensor_right=sensor_back="SN";
            x=y=id=0; item="";
        }
    };

    // brain structure
    struct Vertex2
    {
        std::string type;
        int id;
        // identifies component to which the node is relative
        // if it is a joint: BodyPart_id, if it is a sensor BodySensor_id
        int id_comp;
        std::vector<Vertex2 *> from_nodes;
        std::vector<Vertex2 *> to_nodes;

        Vertex2()
        {
            type=""; id_comp=id=0;
            from_nodes = std::vector<Vertex2 *>();
            to_nodes = std::vector<Vertex2 *>();
        }
    };

    DecodedGeneticString(){
        root = NULL;
        root_brain = NULL;
    }

    DecodedGeneticString::Vertex * getRoot();
    DecodedGeneticString::Vertex2 * getRootBrain();

    // builds graphs
    void decode(GeneticString * gs,
                LSystem LS,
                std::map<std::string, double> params);


private:

    Vertex *root;         // root of the body graph
    Vertex2 *root_brain;  // root of the brain graph

};

#endif //LSYSTEM_PROTO_DECODEDGENOME_H
