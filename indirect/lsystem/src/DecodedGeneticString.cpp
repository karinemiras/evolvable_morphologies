//
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>


#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "LSystem.h"


/**
 * Transforms the main genetic-string of the genome into a graph that logically represents the connections among the components.
 * @param gs - main genetic-string of the genome.
 * @param LS - Lsystem structure containing the alphabet.
 */
void DecodedGeneticString::decode(GeneticString * gs, LSystem LS, std::map<std::string, double> params) {

    int id = 0; // id of the component
    int num_components = 0;
    std::string mountingcommand = "";
    DecodedGeneticString::Vertex  *current_component = NULL;
    GeneticString::Node *current_gs_item;
    current_gs_item = gs->getStart();



    for (int i = 0; i < gs->count(); i++) { // for each item of the main genetic-string


        // if limit number of components has not been reached
        if(num_components < params["max_comps"]-1) {

            // if the item is a letter (component) in the alphabet
            if (LS.getAlphabet().count(current_gs_item->item) > 0) {

                std::string letter = current_gs_item->item;

                // creates new node in the graph with the current letter (component)
                DecodedGeneticString::Vertex *new_component;
                new_component = new DecodedGeneticString::Vertex;
                id++;
                new_component->item = letter;
                new_component->id = id;
                new_component->left = NULL;
                new_component->front = NULL;
                new_component->right = NULL;

                // if there are no components in the graph yet, creates root
                if (root == NULL) {

                    new_component->back = NULL;
                    current_component = new_component;
                    root = new_component;
                    mountingcommand = "";

                } else { // if the graph has been started already

                    // if a command has been set for mounting the component
                    // commands of previous vaiolations remain to pair with a possible new letter, unless a new command be added
                    if (!mountingcommand.empty()) {

                        std::string component = current_component->item.substr(0, 1);

                        // if component is core or brick, having multilateral connection
                        if (component == "C" or component == "B") {


                            // if all sides are occupied in the root, grows to the back
                            if (root == current_component and current_component->left != NULL and

                                current_component->front != NULL and current_component->right != NULL) {

                                if (current_component->back == NULL) {

                                    current_component->back = new_component;
                                } else {

                                    goto violation;
                                }

                            } else { // mounts component in the sue side if theres no violation

                                if (mountingcommand == "l") {  // mounts component on the left
                                    if (current_component->left != NULL) { // if position is occupied

                                        goto violation;
                                    } else {

                                        current_component->left = new_component;
                                    }
                                }

                                if (mountingcommand == "f") {  // mounts component on the front
                                    if (current_component->front != NULL) { // if  position is occupied

                                        goto violation;
                                    } else {

                                        current_component->front = new_component;
                                    }
                                }

                                if (mountingcommand == "r") {  // mounts component on the right
                                    if (current_component->right != NULL) { // if  position is occupied

                                        goto violation;
                                    } else {

                                        current_component->right = new_component;
                                    }
                                }
                            }


                        } else { // if component is a joint

                            if (current_component->front != NULL) {  // if position is occupied, then its a violation

                                goto violation;
                            } else {

                                current_component->front = new_component; // adds component to the front of the joint
                            }

                        }

                        new_component->back = current_component;
                        current_component = new_component;
                        num_components++;

                        violation: int vio=1;// when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored


                    } else {
                        // std::cout<< "-- no mounting-command violation!" << std::endl;
                    }
                }

            // the item is a command
            } else {

                std::string typecommand = current_gs_item->item.substr(0, 4);

                // if it is a moving command
                if (typecommand == "move") {


                    std::string movingcommand = current_gs_item->item.substr(4, 1);

                    if(movingcommand == "b"){
                        // if it is not root, moves back to parent component in the graph
                        if (current_component != root) {

                            current_component = current_component->back;
                        } //else {  std::cout<< "-- back-to-parent violation!" << std::endl;}
                    }

                    if(movingcommand == "l"){
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->left != NULL) {

                            current_component = current_component->left;
                        } //else {  std::cout<< "-- move-to-left violation!" << std::endl;}
                    }

                    if(movingcommand == "f"){
                        // if there is a component to the front, moves to it in the graph
                        if (current_component->front != NULL) {

                            current_component = current_component->front;
                        } //else  { std::cout<< "-- move-to-front violation!" << std::endl;}
                    }

                    if(movingcommand == "r"){
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->right != NULL) {

                            current_component = current_component->right;
                        } //else { std::cout<< "-- move-to-right violation!" << std::endl;}
                    }


                // if it is a mounting command
                } else {

                    // discovers the type of mounting command, to be used with the next component to be mounted later on
                    mountingcommand = current_gs_item->item.substr(3, 1);
                }


            }
            current_gs_item = current_gs_item->next;
        }
    }
}

/**
 * @return pointer to the root of the graph of components
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}