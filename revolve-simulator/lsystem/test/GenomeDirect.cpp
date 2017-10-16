//
// Created by Karine Miras on 07/03/2017.
//


#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <math.h>

#include "Aux.h"
#include "GenomeDirect.h"




/**
* Generates initial genomes for the direct encoding.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void GenomeDirect::build_genome(LSystem LS, std::map<std::string, double> params) {

    std::vector<std::string> alp_i = LS.getAlphabetIndex();
    std::vector<std::string> mountingcom = LS.getMountingCommands();
    std::vector<std::string> movingcom = LS.getMovingCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, (int) params["max_comps"]); // distribution for the number of components
    // distribution for letters of the alphabet, excluding 0 to prevent core-component of being (re)included in the rule
    std::uniform_int_distribution<int> dist_2(1, (int) alp_i.size()-1);
    std::uniform_int_distribution<int> dist_3(0, (int) mountingcom.size()-1); // distribution for the mounting commands
    std::uniform_int_distribution<int> dist_4(0, (int) movingcom.size()-1); // distribution for the moving commands

    std::vector<std::string> letter_items;

    // forces core-component inclusion in the rule as first item of the genetic string
    letter_items.push_back("C");

    // while a raffled number of components is not achieved
    // (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
    int num_items = dist_1(generator);
    while(letter_items.size()  < (num_items*3) ){

        // raffles a letter to be included
        std::string item = alp_i[dist_2(generator)];

        // raffles a mounting command to be included
        letter_items.push_back(mountingcom[dist_3(generator)]);
        // adds letter
        letter_items.push_back(item);

        // a raffles a moving command to be included
        letter_items.push_back(movingcom[dist_4(generator)]);

    }

    // build the genetic-string
    GeneticString gs;
    this->gs = this->build_genetic_string(gs, letter_items);
    this->gs.display_list();

}





/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
* @param argc - command line parameter
* @param argv[] - command line parameter
* @param params - list of params read from configuration file.
* @param LS - Lsystem structure containing the alphabet.
**/

void GenomeDirect::developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation, std::string path) {

    // decodes the final genetic-string into a tree of components
    this->decodeGeneticString(LS, params);

    // generates robot-graphics
    this->constructor(argc, argv, params, path+std::to_string(generation));
}

