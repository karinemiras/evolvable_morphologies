//
// Created by Karine Miras on 08/03/2017.
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>


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
                                  std::map<std::string, double> params) {

    int num_components = 0;
    std::string mountingcommand = "";
    DecodedGeneticString::Vertex  *current_component = NULL;
    GeneticString::Node *current_gs_item;
    current_gs_item = gs->getStart();



    for (int i = 0; i < gs->count(); i++)
    { // for each item of the main genetic-string

        // if limit number of components has not been reached
        if(num_components < params["max_comps"])
        {
            // if the item is a letter (component) in the alphabet
            if (LS.getAlphabet().count(current_gs_item->item) > 0){

                // if item is a sensor
                if (LS.getAlphabetType()[current_gs_item->item] == "sensor")
                {
                    if (root != NULL // if previous component is core or brick
                        and (current_component->item=="C" or current_component->item=="B" ))
                    {
                        // if there's a mounting command
                        if (!mountingcommand.empty()) {

                            if (root == current_component
                                and (current_component->left != NULL or
                                     current_component->sensor_left != "SN")
                                and (current_component->front != NULL or
                                     current_component->sensor_front != "SN")
                                and (current_component->right != NULL or
                                     current_component->sensor_right != "SN")
                                    )
                            {
                                if (current_component->back == NULL and
                                  current_component->sensor_back == "SN") {
                                    current_component->sensor_back = current_gs_item->item;
                                }
                            } else {

                                if (mountingcommand == "l") {
                                    if (current_component->left == NULL and
                                        current_component->sensor_left == "SN")
                                        current_component->sensor_left =
                                                current_gs_item->item;
                                }
                                if (mountingcommand == "f") {
                                    if (current_component->front == NULL and
                                        current_component->sensor_front == "SN")
                                        current_component->sensor_front =
                                                current_gs_item->item;
                                }
                                if (mountingcommand == "r") {
                                    if (current_component->right == NULL and
                                        current_component->sensor_right == "SN")
                                        current_component->sensor_right =
                                                current_gs_item->item;
                                }
                            }

                        }
                    }
                }

                // if item is a body component
                if (LS.getAlphabetType()[current_gs_item->item] == "body") {

                    std::string letter = current_gs_item->item;

                    // creates new node in the graph with the current letter (component)
                    DecodedGeneticString::Vertex *new_component;
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

                            std::string component = current_component->item.substr(0, 1);

                            // if component is core or brick, having multilateral connection
                            if (component == "C" or component == "B") {


                                // if all sides are occupied in the root, grows to the back
                                if (root == current_component
                                    and  (current_component->left != NULL or
                                         current_component->sensor_left != "SN" )
                                    and  (current_component->front != NULL or
                                         current_component->sensor_front != "SN" )
                                    and  (current_component->right != NULL or
                                         current_component->sensor_right != "SN")
                                    )
                                {

                                    if (current_component->back == NULL and
                                            current_component->sensor_back == "SN") {

                                        current_component->back = new_component;
                                    } else {

                                        goto violation;
                                    }

                                } else { // mounts component in the sue side if theres no violation

                                    if (mountingcommand == "l") {  // mounts component on the left
                                        if (current_component->left != NULL
                                            or current_component->sensor_left != "SN")
                                        { // if position is occupied

                                            goto violation;
                                        } else {

                                            current_component->left = new_component;
                                        }
                                    }

                                    if (mountingcommand == "f") {  // mounts component on the front
                                        if (current_component->front != NULL
                                            or current_component->sensor_front != "SN")
                                        { // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->front = new_component;
                                        }
                                    }

                                    if (mountingcommand == "r") {  // mounts component on the right
                                        if (current_component->right != NULL
                                            or
                                                current_component->sensor_right != "SN")
                                        {  // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->right = new_component;
                                        }
                                    }
                                }


                            } else {

                                // if component is a joint
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

                            violation:
                            int vio = 1;// when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored


                        }
                    }
                }

            // the item is a command
            } else {

                std::string typecommand = current_gs_item->item.substr(0, 3);

                // if it is a moving command
                if (typecommand == "mov") {


                    std::string movingcommand = current_gs_item->item.substr(4, 1);

                    if(movingcommand == "b"){
                        // if it is not root, moves back to parent component in the graph
                        if (current_component != root) {

                            current_component = current_component->back;
                        }
                    }

                    if(movingcommand == "l"){
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->left != NULL) {

                            current_component = current_component->left;
                        }
                    }

                    if(movingcommand == "f"){
                        // if there is a component to the front, moves to it in the graph
                        if (current_component->front != NULL) {

                            current_component = current_component->front;
                        }
                    }

                    if(movingcommand == "r"){
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->right != NULL) {

                            current_component = current_component->right;
                        }
                    }

                }

                // if it is a mounting command
                if (typecommand == "add") {

                    // discovers the type of mounting command, to be used with the next component to be mounted later on
                    mountingcommand = current_gs_item->item.substr(3, 1);
                }

                // if it is a brain command
                if (typecommand == "bra") {

                    std::cout<<current_gs_item->item<<std::endl;
                }


            }
            current_gs_item = current_gs_item->next;
        }
    }
}

/**
 * @return pointer to the root of the body graph
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}

/**
 * @return pointer to the root of the brain graph
 */
DecodedGeneticString::Vertex2 * DecodedGeneticString::getRootBrain(){
    return this->root_brain;
}