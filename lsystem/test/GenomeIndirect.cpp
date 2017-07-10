//
// Created by Karine Miras on 07/03/2017.
//


#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <math.h>

#include "Aux.h"
#include "GenomeIndirect.h"


void GenomeIndirect::setGrammar(std::map<std::string, GeneticString> grammar) {
    this->grammar = grammar;
}



/**
* Generates initial production rules (genome) for the alphabet of the indirect encoding.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void GenomeIndirect::build_grammar(LSystem LS, std::map<std::string, double> params) {

    std::map< std::string, std::string > alp = LS.getAlphabet();
    std::vector<std::string> alp_i = LS.getAlphabetIndex();
    std::vector<std::string> mountingcom = LS.getMountingCommands();
    std::vector<std::string> movingcom = LS.getMovingCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, (int) params["num_initial_comp"]); // distribution for the number of components
    std::uniform_int_distribution<int> dist_2(0, (int) alp_i.size()-1); // distribution for letters of the alphabet
    std::uniform_int_distribution<int> dist_3(0, (int) mountingcom.size()-1); // distribution for the mounting commands
    std::uniform_int_distribution<int> dist_4(0, (int) movingcom.size()-1); // distribution for the moving commands

    // for each letter of the alphabet
    for (std::map< std::string, std::string >::const_iterator it = alp.begin(); it != alp.end(); ++it) {

        std::string letter = it->first;

        std::vector<std::string> letter_items;

        if (letter == "C") { // if it is a core component, forces its inclusion in the rule
            letter_items.push_back(letter);
        }

        // while a raffled number of components is not achieved
        // (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
        int num_items = dist_1(generator);
        while(letter_items.size() < (num_items*3) ){

            // raffles a letter to be included
            std::string item = alp_i[dist_2(generator)];

            // prevents core component of being (re)included in the rule
            if (item != "C") {

                // raffles a mounting command to be included
                letter_items.push_back(mountingcom[dist_3(generator)]);
                // adds letter
                letter_items.push_back(item);

                // a raffles a moving command to be included
                letter_items.push_back(movingcom[dist_4(generator)]);

            }
        }

        // build a genetic-string with the new production rule
        GeneticString lgs;
        lgs = this->build_genetic_string(lgs, letter_items);

        // adds letter and its production rule (made a genetic-string) to the grammar of the genome
        this->grammar.emplace(letter, lgs);

    }

}





/**
 * Genome development: performs replacements in the main genetic-string with production rules for a number of iterations.
 * @param replacement_iterations - number of iterations to perform replacements.
 * @param export_genomes - flag for exporting the main genetic-string to a file.
 */
void GenomeIndirect::generate_final_string(int  replacement_iterations, int export_genomes, int generation, std::string path){

    // performs replacements for a number of iterations
    for(int i=1; i<=replacement_iterations ;i++) {

        // replacement is made given the grammar
        this->gs.replaces(this->grammar);
    }

    //this->gs.display_list();

    if(export_genomes == 1){
        this->exportGenome(path+std::to_string(generation));
    }
}


/**
*  Creates the genome of an individual in its initial state, as a genetic-string formed only by the axiom.
**/
void GenomeIndirect::createEmbryo(){

    std::vector<std::string> axiom;
    axiom.push_back("C");

    // initializes the genetic-string with the axiom
    GeneticString gs;
    this->setGeneticString(this->build_genetic_string(gs, axiom));

}




/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
* @param argc - command line parameter
* @param argv[] - command line parameter
* @param params - list of params read from configuration file.
* @param LS - Lsystem structure containing the alphabet.
**/

void GenomeIndirect::developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation, std::string path) {

    // creates main genetic-string for axiom (initial developmental state of the genome)
    this->createEmbryo();

    // enhances the genetic-string according to grammar iteratively
    this->generate_final_string((int) params["replacement_iterations"], (int) params["export_genomes"], generation, path);

    // decodes the final genetic-string into a tree of components
    this->decodeGeneticString(LS, params);

    // generates robot-graphics
    this->constructor(argc, argv, params, path+std::to_string(generation));
}




std::map< std::string, GeneticString > Genome::getGrammar(){

    return this->grammar;
}


