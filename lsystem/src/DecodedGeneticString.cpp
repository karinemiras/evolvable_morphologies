//
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>


#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "LSystem.h"


void DecodedGeneticString::decode(GeneticString gs, LSystem LS) {

    int id = 0;
    std::string command = "";
    DecodedGeneticString::node  *current_component = NULL;
    GeneticString::node *current_gs_item;// = new GeneticString::node;
    current_gs_item = gs.getStart();


    for (int i = 0; i < gs.count(); i++) { // for each item of the genetic string

        std::cout<<std::endl<<"item: "<<LS.getAlphabet()[current_gs_item->item]<< " previous command: "<<command<<std::endl;

        if (LS.getAlphabet().count(current_gs_item->item) > 0) {  // if the item is a component in the alphabet

            std::string letter = LS.getAlphabet()[current_gs_item->item];
            std::cout << letter << " - comp is alpha" << std::endl;

            DecodedGeneticString::node *new_component;
            new_component = new DecodedGeneticString::node;
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

                    if (component == "C" or component == "B") { // if component is core or brick, having multilateral connexion

                        std::string left = current_component->item.substr(1, 1);
                        std::string front = current_component->item.substr(2, 1);
                        std::string right = current_component->item.substr(3, 1);

                        std::cout << "multi parent-" << component <<left<< front<< right << std::endl;

                        if(root == current_component and current_component->left != NULL and current_component->front != NULL and current_component->right != NULL ){ // if all sides are occupied in the root, grows to the back

                            if(current_component->back == NULL){
                                std::cout << " back " << std::endl;
                                current_component->back = new_component;
                            }else{
                                std::cout << "--root-back violation!" << std::endl;
                                goto violation;
                            }

                        }else {

                            if (command == "l") {  // mount on the left
                                if (left != "N" or
                                    current_component->left != NULL) { // if theres a sensor or position is occupied
                                    std::cout << "--left violation!" << std::endl;
                                    goto violation;
                                } else {
                                    std::cout << " left " << std::endl;
                                    current_component->left = new_component;
                                }
                            }
                            if (command == "f") {  // mount on the front
                                if (front != "N" or
                                    current_component->front != NULL) { // if theres a sensor or position is occupied
                                    std::cout << "--front violation!" << std::endl;
                                    goto violation;
                                } else {
                                    std::cout << " front " << std::endl;
                                    current_component->front = new_component;
                                }
                            }
                            if (command == "r") {  // mount on the right
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

                        if(current_component->front != NULL){  // if position is occupied
                                std::cout<< "--front violation!" << std::endl;
                                goto violation;
                        }else{
                                std::cout<<" front "<<std::endl;
                                current_component->front = new_component;
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

            command = current_gs_item->item.substr(current_gs_item->item.size()-1,1);
            std::cout<<current_gs_item->item<<" "<<command << "-- not alpha" << std::endl;

            if(command == "<"){ // if it is a move-command, move back to parent in the graph, if it is not on root
                if (current_component != root) { // root has no parent, so the back is another branch
                    current_component = current_component->back;
                    command = "";
                    std::cout << " move back to parent " << current_component->item<<std::endl;
                }else{
                    std::cout<< "-- back-to-parent violation!" << std::endl;
                }
            }
        }
        current_gs_item = current_gs_item->next;
    }
}

DecodedGeneticString::node * DecodedGeneticString::getRoot(){

    return this->root;

}