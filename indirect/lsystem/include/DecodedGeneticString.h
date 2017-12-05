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

  DecodedGeneticString(){
      root = NULL;
      ids=0;
  }

  ~DecodedGeneticString(){}

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
            sensor_left=sensor_front=sensor_right=sensor_back="Sn";
            x=y=id=0; item="";
        }
    };

    // brain structure
    struct Vertex2
    {
        int id;
        // identifies component (joint or sensor) to which the node is relative
        int id_comp;
        double bias;
        double amplitude;
        double period;
        double phase_offset;
        double weight;
        std::string layer;
        std::string function;
        std::string direction;
        std::vector<Vertex2 *> from_nodes;
        std::vector<Vertex2 *> to_nodes;

        Vertex2()
        {
            layer=function=direction=""; id_comp=id=-1;
            weight=bias=amplitude=period=phase_offset=0;
            from_nodes = std::vector<Vertex2 *>();
            to_nodes = std::vector<Vertex2 *>();
        }
    };



    DecodedGeneticString::Vertex * getRoot();

    void decodeBrainNode(std::string direction,
                         std::string item,
                         int id_comp,
                         std::string path);

    void decodeBrainCommand(std::string item,
                            std::string path);

    // builds graphs
    void decode(GeneticString gs,
                LSystem LS,
                std::map<std::string, double> params,
                std::string path);

    std::map< std::pair<int, int>, double >
                        getBrain_edges();

    std::map< int, Vertex2 * > getBrain_nodes();


private:

    Vertex *root;  // root of the body graph

    // edges of brain graph
    std::map< std::pair<int, int>, double > // <<origin, destination>, weight>
            brain_edges = std::map< std::pair<int, int>, double >();

    // nodes of brain graph
    std::map< int, Vertex2 * >
            brain_nodes = std::map< int, Vertex2 *>();

    // pointers to current-edge of brain graph:

    //  'from' node(s) in current-edge
    std::vector<Vertex2 *> fromNode =
        std::vector<Vertex2 *>();

    //  'to' node(s) in current-edge
    std::vector<Vertex2 *> toNode =
        std::vector<Vertex2 *>();

    int ids; // count of ids so far
};

#endif //LSYSTEM_PROTO_DECODEDGENOME_H