//
// Created by Karine Miras on 08/03/2017.
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "LSystem.h"


/**
 * Transforms the main genetic-string of the genome into graphs that
 * logically represent the connections among the components of body and brain.
 * @param gs - main genetic-string of the genome.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
 */
void DecodedGeneticString::decode(GeneticString * gs,
                                  LSystem LS,
                                  std::map<std::string, double> params,
                                  std::string path)
{


    std::ofstream file;
    file.open("../../experiments/"+ path +"/tempbrain.dot");
    file<<" digraph g{"<<std::endl;
    file.close();

    int num_components = 0;
    std::string mountingcommand = "";
    DecodedGeneticString::Vertex  *current_component = NULL;
    DecodedGeneticString::Vertex *new_component = NULL;
    GeneticString::Node *current_gs_item;
    current_gs_item = gs->getStart();


    for (int i = 0; i < gs->count(); i++)
    { // for each item of the main genetic-string

        std::cout<<"current_gs_item "<<current_gs_item->item<<std::endl;


        // if the item is a letter (component) in the alphabet
        if (LS.getAlphabet().count(current_gs_item->item) > 0){

            // if limit number of components has not been reached
            if(num_components < params["max_comps"])
            {

                // if item is a sensor
                if (LS.getAlphabetType()[current_gs_item->item] == "sensor") {
                    if (root != NULL // if previous component is core or brick
                        and (current_component->item == "C" or
                             current_component->item == "B")) {
                        // if there's a mounting command
                        if (!mountingcommand.empty()) {

                            if (root == current_component
                                and (current_component->left != NULL or
                                     current_component->sensor_left != "Sn")
                                and (current_component->front != NULL or
                                     current_component->sensor_front != "Sn")
                                and (current_component->right != NULL or
                                     current_component->sensor_right != "Sn")
                                    ) {
                                if (current_component->back == NULL and
                                    current_component->sensor_back == "Sn") {
                                    current_component->sensor_back = current_gs_item->item;
                                    this->decodeBrainNode
                                            (current_gs_item->item,
                                             current_component->id, path);
                                }
                            } else {

                                if (mountingcommand == "l") {
                                    if (current_component->left == NULL and
                                        current_component->sensor_left ==
                                        "Sn") {
                                        current_component->sensor_left = current_gs_item->item;
                                        this->decodeBrainNode
                                                (current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                                if (mountingcommand == "f") {
                                    if (current_component->front == NULL and
                                        current_component->sensor_front ==
                                        "Sn") {
                                        current_component->sensor_front = current_gs_item->item;
                                        this->decodeBrainNode
                                                (current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                                if (mountingcommand == "r") {
                                    if (current_component->right == NULL and
                                        current_component->sensor_right ==
                                        "Sn") {
                                        current_component->sensor_right = current_gs_item->item;
                                        this->decodeBrainNode
                                                (current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                            }

                        }
                    }
                }

                // if item is a body component
                if (LS.getAlphabetType()[current_gs_item->item] == "body") {

                    std::string letter = current_gs_item->item;

                    // creates new node in the graph with the current letter (component)
                    new_component = new DecodedGeneticString::Vertex;
                    new_component->item = letter;
                    new_component->id = num_components;
                    new_component->left = NULL;
                    new_component->front = NULL;
                    new_component->right = NULL;

                    // if there are no components in the graph yet, creates root
                    if (root == NULL) {

                        new_component->back = NULL;
                        current_component = new_component;
                        num_components++;
                        root = new_component;
                        mountingcommand = "";

                    } else { // if the graph has been started already

                        // if a command has been set for mounting the component
                        // commands of previous violations remain to pair with a possible new letter, unless a new command be added
                        if (!mountingcommand.empty()) {

                            std::string component = current_component->item.substr(
                                    0, 1);

                            // if component is core or brick, having multilateral connection
                            if (component == "C" or component == "B") {


                                // if all sides are occupied in the root, grows to the back
                                if (root == current_component
                                    and (current_component->left != NULL or
                                         current_component->sensor_left != "Sn")
                                    and (current_component->front != NULL or
                                         current_component->sensor_front !=
                                         "Sn")
                                    and (current_component->right != NULL or
                                         current_component->sensor_right !=
                                         "Sn")
                                        ) {

                                    if (current_component->back == NULL and
                                        current_component->sensor_back ==
                                        "Sn") {

                                        current_component->back = new_component;
                                    } else {

                                        goto violation;
                                    }

                                } else { // mounts component in the sue side if theres no violation

                                    if (mountingcommand ==
                                        "l") {  // mounts component on the left
                                        if (current_component->left != NULL
                                            or current_component->sensor_left !=
                                               "Sn") { // if position is occupied

                                            goto violation;
                                        } else {

                                            current_component->left = new_component;
                                        }
                                    }

                                    if (mountingcommand ==
                                        "f") {  // mounts component on the front
                                        if (current_component->front != NULL
                                            or
                                            current_component->sensor_front !=
                                            "Sn") { // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->front = new_component;
                                        }
                                    }

                                    if (mountingcommand ==
                                        "r") {  // mounts component on the right
                                        if (current_component->right != NULL
                                            or
                                            current_component->sensor_right !=
                                            "Sn") {  // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->right = new_component;
                                        }
                                    }
                                }


                            } else {

                                // if current-component(parent) is a joint
                                // (always mounts on the front)
                                if (component == "A") {

                                    if (current_component->front !=
                                        NULL) {  // if position is occupied, then its a violation

                                        goto violation;
                                    } else {

                                        current_component->front = new_component; // adds component to the front of the joint
                                    }
                                }


                            }

                            new_component->back = current_component;
                            current_component = new_component;
                            num_components++;
                            mountingcommand = "";

                            if (new_component->item.substr(0, 1) == "A") {
                                this->decodeBrainNode(current_gs_item->item,
                                                      new_component->id, path);
                            }

                            violation:
                            int vio = 1;// when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored


                        }
                    }
                }
            }

            // the item is a command
        } else {

                std::string typecommand = current_gs_item->item.substr(0, 3);

                // if it is a moving command
                if (typecommand == "mov") {


                    std::string movingcommand = current_gs_item->item.substr(4, 1);

                    // if new component is a joint
                    // left/front/right move to the front
                    if(current_component->item.substr(0, 1) == "A"){

                        if (movingcommand == "b") {
                                current_component = current_component->back;
                        }else {

                            if (current_component->front != NULL) {

                                current_component = current_component->front;
                            }
                        }

                    }else {

                        if (movingcommand == "b") {
                            // if it is not root, moves back to parent component in the graph
                            if (current_component != root) {

                                current_component = current_component->back;
                            }
                        }

                        if (movingcommand == "l") {
                            // if there is a component to the left, moves to it in the graph
                            if (current_component->left != NULL) {

                                current_component = current_component->left;
                            }
                        }

                        if (movingcommand == "f") {
                            // if there is a component to the front, moves to it in the graph
                            if (current_component->front != NULL) {

                                current_component = current_component->front;
                            }
                        }

                        if (movingcommand == "r") {
                            // if there is a component to the left, moves to it in the graph
                            if (current_component->right != NULL) {

                                current_component = current_component->right;
                            }
                        }
                    }

                }

                // if it is a mounting command
                if (typecommand == "add")
                {
                    // discovers the type of mounting command, to be used with the next component to be mounted later on
                    mountingcommand = current_gs_item->item.substr(3, 1);
                }

                // if it is a brain command
                if (typecommand == "bra")
                {
                    this->decodeBrainCommand(current_gs_item->item, path);
                }


            }
            current_gs_item = current_gs_item->next;
    }


    file.open("../../experiments/"+ path +"/tempbrain.dot", std::ofstream::app);
    for (const auto &c : this->brain_graph){
        file<<c.first.first<<" -> "<<c.first.second<<";"<<std::endl;
    }
    file<<" }"<<std::endl;
    file.close();

}

/** Decodes commands of the alphabet to alter the brain graph.
 * @param item - item of the alphabet
 * @path - directory to salve files
 * */
void DecodedGeneticString::decodeBrainCommand(std::string item,
                                              std::string path)
{
    // if there is a current-edge
    if(this->toNode != NULL) {

        std::vector<std::string> tokens;
        boost::split(tokens, item,  boost::is_any_of("_"));
        auto command = tokens[0];
        auto param = std::stod(tokens[1]);

        auto edge = std::make_pair(
                this->fromNode[0]->id,
                this->toNode->id);

        std::cout<<" ----------before---------- "<<std::endl;
        for (const auto &c : this->brain_graph){
            auto from = c.first.first;
            auto to = c.first.second;
            auto weight = c.second;
            std::cout<<from<<" "<<to<<" "<<weight<<std::endl;
        }

        // pertubs weight of current-edge
        if(command == "brainperturb")
        {
                this->brain_graph[edge] = this->brain_graph[edge]
                                          + param;
                if(this->brain_graph[edge]>1) this->brain_graph[edge] = 1;
                if(this->brain_graph[edge]<-1) this->brain_graph[edge] = -1;
        }

        // creates self-connection
        if(command == "brainloop")
        {
            auto self_edge = std::make_pair(
                    this->toNode->id,
                    this->toNode->id);
            // if self-connection does not exist already
            if(this->brain_graph.count(self_edge) == 0)
            {
                this->brain_graph[self_edge] = param;
            }
        }

        std::cout<<" ----------after---------- "<<std::endl;
        for (const auto &nothing : this->brain_graph){
            std::cout<<nothing.first.first<<" "<<nothing.first.second<<" "<<nothing.second<<std::endl;
        }
    }
}

/** Decodes letters (sensors and joints) of the alphabet into part of the brain
 * graph.
 * @param item - item of the alphabet
 * @param id_comp - if of the body component to which this node is related to
 * @path - directory to salve files
 * */
void DecodedGeneticString::decodeBrainNode(std::string item,
                                           int id_comp,
                                           std::string path){

    std::ofstream file;
    file.open("../../experiments/"+ path +"/tempbrain.dot", std::ofstream::app);

    DecodedGeneticString::Vertex2 * v = new Vertex2();
    this->ids++;
    v->id = this->ids;
    v->id_comp = id_comp;
    v->type = item;

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> weight_uni(-1, 1);

    // the item is a sensor
    if(item == "SI" or item == "SL")
    {
        // if there's no output node yet
        // adds node to the list of 'from' nodes of current-edge
        // remember: list starts with bias
        if (this->toNode == NULL)
        {
            this->fromNode.push_back(v);
        }else
        {
            // update 'from' of current-edge
            this->fromNode[0] = v;

            // connects new node to 'to-node' of current edge
            this->toNode->from_nodes.push_back(this->fromNode[0]);
            this->fromNode[0]->to_nodes.push_back(this->toNode);

            std::pair<int, int> edge = std::make_pair(this->fromNode[0]->id,
                                                     this->toNode->id);
            this->brain_graph[edge] = weight_uni(generator);

        }

        if (item == "SL") {
            file << v->id << "[color=\"lightgrey\",style=filled];"<<std::endl;
        }
        if (item == "SI") {
            file << v->id << "[color=\"grey\",style=filled];"<<std::endl;
        }
    }

    // the item is an active joint
    if(item == "AJ1" or item == "AJ2")
    {
        // update 'to' of current-edge
        this->toNode = v;

        // if theres a list, connects new node to them all and updates the list
        // with the latest node only
        for(int i=0; i< this->fromNode.size(); i++){

            // connects new node to 'from-node(s)' of current edge
            this->toNode->from_nodes.push_back(this->fromNode[i]);
            this->fromNode[i]->to_nodes.push_back(this->toNode);


            std::pair<int, int> edge = std::make_pair(this->fromNode[i]->id,
                                                     this->toNode->id);
            this->brain_graph[edge] = weight_uni(generator);
        }

        file << this->toNode->id << " [shape=box,color=\"red\",style=filled];"<<std::endl;

        // remove from list after connection, except for the last
        this->fromNode.erase(this->fromNode.begin(),
                             this->fromNode.begin()+this->fromNode.size()-1);
    }



    std::cout<<" -------------------- "<<item<<" "<<id_comp<<std::endl;

    for (const auto &c : this->brain_graph){
        std::cout<<c.first.first<<" "<<c.first.second<<" "<<c.second<<std::endl;
    }

    std::cout<<"> from list: "<<std::endl;
    for(int i=0;i<this->fromNode.size();i++){
        std::cout<<std::endl<<" id "<<this->fromNode[i]->id;
        std::cout<<" type "<<this->fromNode[i]->type;
        std::cout<<" idcomp "<<this->fromNode[i]->id_comp<<std::endl;
        if(this->fromNode[i]->to_nodes.size()>0){
            for(int j=0;j<this->fromNode[i]->to_nodes.size();j++){
                std::cout<<"   id "<<this->fromNode[i]->to_nodes[j]->id;
                std::cout<<" type "<<this->fromNode[i]->to_nodes[j]->type;
                std::cout<<" idcomp "<<this->fromNode[i]->to_nodes[j]->id_comp<<std::endl;
            }
        }
    }

    std::cout<<"> to  : "<<std::endl;
    if (this->toNode != NULL) {
        std::cout << std::endl << " id " << this->toNode->id;
        std::cout << " type " << this->toNode->type;
        std::cout << " idcomp " << this->toNode->id_comp << std::endl;
        if (this->toNode->from_nodes.size() > 0) {
            for (int j = 0; j < this->toNode->from_nodes.size(); j++) {
                std::cout  << "   id "
                          << this->toNode->from_nodes[j]->id;
                std::cout << " type " << this->toNode->from_nodes[j]->type;
                std::cout << " idcomp "
                          << this->toNode->from_nodes[j]->id_comp << std::endl;
            }
        }

    }

    file.close();
}

/**
 * @return pointer to the root of the body graph
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}

