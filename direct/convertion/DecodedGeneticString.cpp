//
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>

#include <fstream>

#include "DecodedGeneticString.h"


#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <map>
#include <vector>


//#include "GeneticString.h"
//#include "LSystem.h"




/**
 * Transforms the main genetic-string of the genome into a graph that logically represents the connections among the components.
 * @param gs - main genetic-string of the genome.
 * @param LS - Lsystem structure containing the alphabet.
 */
void DecodedGeneticString::decode(std::string genome_file, std::map<std::string, double> params) {
//void DecodedGeneticString::decode(GeneticString gs, LSystem LS, std::map<std::string, double> params) {

    std::string line;
    std::ifstream file;

    file.open(genome_file);
    if (!file.is_open()) {
        std::cout << "Could not open robot text file " << std::endl;

    }else {

        int id = 0;
        int num_components = 0;
        DecodedGeneticString::Vertex  *current_component = NULL;

        std::map<std::string, std::string> letters;
        letters["CoreComponent"] = "C";
        letters["FixedBrick"] = "B";
        letters["ActiveHinge"] = "AJ2";

        int position = 0;
        std::string letter = "";
        int level = 0, previous_level = 0;


        while (getline(file, line)) {

            std::cout << ">>>>>>> " << line << std::endl << std::endl << std::endl;

            if(num_components <= params["max_comps"]) {

                std::vector<std::string> tokens, tokens_level;
                boost::split(tokens_level, line, boost::is_any_of("\t"));
                int level = tokens_level.size();

                boost::split(tokens, line, boost::is_any_of(" "));

                position = std::stoi(tokens[0]);
                letter = tokens[1];
                std::string mounting_command = "";

                DecodedGeneticString::Vertex *new_component;
                new_component = new DecodedGeneticString::Vertex;
                id++;
                new_component->item = letters[letter];
                new_component->id = id;
                new_component->left = NULL;
                new_component->front = NULL;
                new_component->right = NULL;

  std::cout<<level<<" "<<previous_level<<std::endl;



                std::cout << "position " << position << " letter " << letter <<std::endl;


                if (level < previous_level) {

                    int num = (int) (previous_level - level)+1;

                    for (int j = 1; j <= num; j++) {
                        current_component = current_component->back;

                    }
                }


                if (root == NULL) {

                    new_component->back = NULL;
                    current_component = new_component;
                    root = new_component;

                } else {

                    // decides which mouting command to use
                    if (current_component->item == "C") {

                        if (position == 0) mounting_command = "back";
                        if (position == 1) mounting_command = "addl";
                        if (position == 2) mounting_command = "addf";
                        if (position == 3) mounting_command = "addr";
                    }

                    if (current_component->item == "B") {

                        if (position == 0) mounting_command = "addf";
                        if (position == 1) mounting_command = "addl";
                        if (position == 2) mounting_command = "addr";
                    }

                    if (current_component->item == "AJ2") mounting_command = "addf";


                    // mounts the component in the right position
                    if (mounting_command == "back") {
                        current_component->back = new_component;
                    }

                    if (mounting_command == "addf") {
                        current_component->front = new_component;
                    }

                    if (mounting_command == "addl") {
                        current_component->left = new_component;
                    }

                    if (mounting_command == "addr") {
                        current_component->right = new_component;
                    }
std::cout<<"mouting:"<<mounting_command<<std::endl;

                    new_component->back = current_component;
                    current_component = new_component;


//                    if (level > previous_level){
//                        level_letters.push_back(letter);
//                    }
//
//                    if (level < previous_level) {
//
//                        int num = (int) (previous_level - level)+1;
//
//                        for (int j = 1; j <= num; j++) {
//
//                            if (level_letters.size() > 2) {
//                                level_letters.erase(level_letters.begin() + level_letters.size() - 1);
//                            }
//                        }
//                    }

                    previous_level = level;

                }

                num_components++;


            }
            std::cout<<"item:"<<current_component->item<<std::endl;
            std::cout<<"current:"<<current_component<<std::endl;
            std::cout<<"current left:"<<current_component->left<<std::endl;
            std::cout<<"current front:"<<current_component->front<<std::endl;
            std::cout<<"current right:"<<current_component->right<<std::endl;
            std::cout<<"PARENT:"<<current_component->back<<std::endl;
            if(root!=current_component) {
                std::cout << "parent left:" << current_component->back->left << std::endl;
                std::cout << "parent front:" << current_component->back->front << std::endl;
                std::cout << "parent right:" << current_component->back->right << std::endl;
            }
        }
    }


//
//
//    se position == last position
//    Add node in position side given last letter

//    int id = 0; // id of the component
//    int num_components = 0;
//    std::string mountingcommand = "";
//    DecodedGeneticString::Vertex  *current_component = NULL;
//    GeneticString::Node *current_gs_item;
//    current_gs_item = gs.getStart();
//
//
//
//    for (int i = 0; i < gs.count(); i++) { // for each item of the main genetic-string
//
//
//        // if limit number of components has not been reached
//        if(num_components < params["max_comps"]-1) {
//
//            // if the item is a letter (component) in the alphabet
//            if (LS.getAlphabet().count(current_gs_item->item) > 0) {
//
//                std::string letter = current_gs_item->item;
//
//                // creates new node in the graph with the current letter (component)
//                DecodedGeneticString::Vertex *new_component;
//                new_component = new DecodedGeneticString::Vertex;
//                id++;
//                new_component->item = letter;
//                new_component->id = id;
//                new_component->left = NULL;
//                new_component->front = NULL;
//                new_component->right = NULL;
//
//                // if there are no components in the graph yet, creates root
//                if (root == NULL) {
//
//                    new_component->back = NULL;
//                    current_component = new_component;
//                    root = new_component;
//                    mountingcommand = "";
//
//                } else { // if the graph has been started already
//
//                    // if a command has been set for mounting the component
//                    // commands of previous vaiolations remain to pair with a possible new letter, unless a new command be added
//                    if (!mountingcommand.empty()) {
//
//                        std::string component = current_component->item.substr(0, 1);
//
//                        // if component is core or brick, having multilateral connection
//                        if (component == "C" or component == "B") {
//
//
//                            // if all sides are occupied in the root, grows to the back
//                            if (root == current_component and current_component->left != NULL and
//
//                                current_component->front != NULL and current_component->right != NULL) {
//
//                                if (current_component->back == NULL) {
//
//                                    current_component->back = new_component;
//                                } else {
//
//                                    goto violation;
//                                }
//
//                            } else { // mounts component in the sue side if theres no violation
//
//                                if (mountingcommand == "l") {  // mounts component on the left
//                                    if (current_component->left != NULL) { // if position is occupied
//
//                                        goto violation;
//                                    } else {
//
//                                        current_component->left = new_component;
//                                    }
//                                }
//
//                                if (mountingcommand == "f") {  // mounts component on the front
//                                    if (current_component->front != NULL) { // if  position is occupied
//
//                                        goto violation;
//                                    } else {
//
//                                        current_component->front = new_component;
//                                    }
//                                }
//
//                                if (mountingcommand == "r") {  // mounts component on the right
//                                    if (current_component->right != NULL) { // if  position is occupied
//
//                                        goto violation;
//                                    } else {
//
//                                        current_component->right = new_component;
//                                    }
//                                }
//                            }
//
//
//                        } else { // if component is a joint
//
//                            if (current_component->front != NULL) {  // if position is occupied, then its a violation
//
//                                goto violation;
//                            } else {
//
//                                current_component->front = new_component; // adds component to the front of the joint
//                            }
//
//                        }
//
//                        new_component->back = current_component;
//                        current_component = new_component;
//                        num_components++;
//
//                        violation: int vio=1;// when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored
//
//
//                    } else {
//                        // std::cout<< "-- no mounting-command violation!" << std::endl;
//                    }
//                }
//
//            // the item is a command
//            } else {
//
//                std::string typecommand = current_gs_item->item.substr(0, 4);
//
//                // if it is a moving command
//                if (typecommand == "move") {
//
//
//                    std::string movingcommand = current_gs_item->item.substr(4, 1);
//
//                    if(movingcommand == "b"){
//                        // if it is not root, moves back to parent component in the graph
//                        if (current_component != root) {
//
//                            current_component = current_component->back;
//                        } //else {  std::cout<< "-- back-to-parent violation!" << std::endl;}
//                    }
//
//                    if(movingcommand == "l"){
//                        // if there is a component to the left, moves to it in the graph
//                        if (current_component->left != NULL) {
//
//                            current_component = current_component->left;
//                        } //else {  std::cout<< "-- move-to-left violation!" << std::endl;}
//                    }
//
//                    if(movingcommand == "f"){
//                        // if there is a component to the front, moves to it in the graph
//                        if (current_component->front != NULL) {
//
//                            current_component = current_component->front;
//                        } //else  { std::cout<< "-- move-to-front violation!" << std::endl;}
//                    }
//
//                    if(movingcommand == "r"){
//                        // if there is a component to the left, moves to it in the graph
//                        if (current_component->right != NULL) {
//
//                            current_component = current_component->right;
//                        } //else { std::cout<< "-- move-to-right violation!" << std::endl;}
//                    }
//
//
//                // if it is a mounting command
//                } else {
//
//                    // discovers the type of mounting command, to be used with the next component to be mounted later on
//                    mountingcommand = current_gs_item->item.substr(3, 1);
//                }
//
//
//            }
//            current_gs_item = current_gs_item->next;
//        }
//    }
}

/**
 * @return pointer to the root of the graph of components
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}