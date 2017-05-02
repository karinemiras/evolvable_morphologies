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
void DecodedGeneticString::decode(GeneticString gs, LSystem LS) {

    int id = 0; // id of the component
    std::string command = "";
    DecodedGeneticString::Vertex  *current_component = NULL;
    GeneticString::Node *current_gs_item;
    current_gs_item = gs.getStart();


    for (int i = 0; i < gs.count(); i++) { // for each item of the main genetic-string

        std::cout<<std::endl<<"item: "<<LS.getAlphabet()[current_gs_item->item]<< " previous command: "<<command<<std::endl;

        if (LS.getAlphabet().count(current_gs_item->item) > 0) {  // if the item is a letter (component) in the alphabet

            std::string letter = current_gs_item->item;
            std::cout << letter << " - comp is alpha" << std::endl;

            // creates new node in the graph with the current letter (component)
            DecodedGeneticString::Vertex *new_component;
            new_component = new DecodedGeneticString::Vertex;
            id ++;
            new_component->item = letter;
            new_component->id = id;
            new_component->left = NULL;
            new_component->front = NULL;
            new_component->right = NULL;

            if (root == NULL) { // if there are no components in the graph yet, creates root
                std::cout<<"root"<<std::endl;

                new_component->back = NULL;
                current_component = new_component;
                root = new_component;
                command = "";

            }else{ // if the graph has been started already

                if (!command.empty()){ // if a command has been set for mounting the component

                    std::string component = current_component->item.substr(0, 1);

                    if (component == "C" or component == "B") { // if component is core or brick, having multilateral connection

                         // maps sensors of the component
                        std::string left = current_component->item.substr(1, 1);
                        std::string front = current_component->item.substr(2, 1);
                        std::string right = current_component->item.substr(3, 1);

                        std::cout << "multi parent-" << component <<left<< front<< right << std::endl;

                        // if all sides are occupied in the root, grows to the back
                        if(root == current_component and current_component->left != NULL and current_component->front != NULL and current_component->right != NULL ){

                            if(current_component->back == NULL){
                                std::cout << " back " << std::endl;
                                current_component->back = new_component;
                            }else{
                                std::cout << "--root-back violation!" << std::endl;
                                goto violation;
                            }

                        }else { // mounts component in the sue side if theres no violation

                            if (command == "l") {  // mounts component on the left
                                if (left != "N" or
                                    current_component->left != NULL) { // if theres a sensor or position is occupied
                                    std::cout << "--left violation!" << std::endl;
                                    goto violation;
                                } else {
                                    std::cout << " left " << std::endl;
                                    current_component->left = new_component;
                                }
                            }

                            if (command == "f") {  // mounts component on the front
                                if (front != "N" or
                                    current_component->front != NULL) { // if theres a sensor or position is occupied
                                    std::cout << "--front violation!" << std::endl;
                                    goto violation;
                                } else {
                                    std::cout << " front " << std::endl;
                                    current_component->front = new_component;
                                }
                            }

                            if (command == "r") {  // mounts component on the right
                                if (right != "N" or
                                    current_component->right != NULL) { // if theres a sensor or position is occupied
                                    std::cout << "--right violation!" << std::endl;
                                    goto violation;
                                } else {
                                    std::cout << " right " << std::endl;
                                    current_component->right = new_component;
                                }
                            }
                        }


                    }else{ // if component is a joint
                        std::cout << "comp joint-" << component << std::endl;

                        if(current_component->front != NULL){  // if position is occupied, then its a violation
                                std::cout<< "--front violation!" << std::endl;
                                goto violation;
                        }else{
                                std::cout<<" front "<<std::endl;
                                current_component->front = new_component; // adds component to the front of the joint
                        }

                    }

                    new_component->back = current_component;
                    current_component = new_component;

                    violation: // when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored
                    command = "";

                }else{
                    std::cout<< "-- no-command violation!" << std::endl;
                }
            }

        } else { // the item is a command

            if(current_gs_item->item.substr(current_gs_item->item.size()-1,1) == "<"){ // if it is a move-command

                if (current_component != root) { // if it is not root, moves back to parent component in the graph

                    current_component = current_component->back;
                    std::cout << " move back to parent " << current_component->item<<std::endl;
                }else{
                    std::cout<< "-- back-to-parent violation!" << std::endl;
                }
            }else{ // if it is a mounting command

                command = current_gs_item->item.substr(current_gs_item->item.size()-1,1); // discovers the type of command, to be used with the next component to be mounted later on
                std::cout<<current_gs_item->item<<" "<<command << "-- not alpha" << std::endl;
            }


        }
        current_gs_item = current_gs_item->next;
    }
}

/**
 * @return pointer to the root of the graph of components
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}