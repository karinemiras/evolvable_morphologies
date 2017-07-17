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


/**
 * Transforms the main genetic-string of the genome into a graph that logically represents the connections among the components.
 * @param gs - main genetic-string of the genome.
 * @param LS - Lsystem structure containing the alphabet.
 */
void DecodedGeneticString::decode(std::string genome_file, std::map<std::string, double> params) {


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


        while (getline(file, line) and line != "") {


            if(num_components <= params["max_comps"]) {

                std::vector<std::string> tokens, tokens_level;
                boost::split(tokens_level, line, boost::is_any_of("\t"));
                level = tokens_level.size();

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


                if (level < previous_level) {

                    int num = (int) (previous_level - level)+1;

                    for (int j = 1; j <= num; j++)
                        current_component = current_component->back;
                }


                if (root == NULL) {

                    new_component->back = NULL;
                    current_component = new_component;
                    root = new_component;

                } else {

                    // decides which mounting command to use

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

                    if (mounting_command == "back") current_component->back = new_component;

                    if (mounting_command == "addf") current_component->front = new_component;

                    if (mounting_command == "addl") current_component->left = new_component;

                    if (mounting_command == "addr") current_component->right = new_component;


                    new_component->back = current_component;
                    current_component = new_component;

                    previous_level = level;

                }

                num_components++;

            }
        }
    }

}

/**
 * @return pointer to the root of the graph of components
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}