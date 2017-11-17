//
// Created by Karine Miras on 07/03/2017.
//

#include "LSystem.h"


/**
 * Builds a vector with the body mounting commands for the l-system.
 */
void LSystem::build_mounting_commands(){

    mounting_commands.push_back("addl"); // add component to the left side of the parent-reference
    mounting_commands.push_back("addf"); //  add component to the front side of the parent-reference
    mounting_commands.push_back("addr"); // add component to the right side of the parent-reference
}

/**
 * Builds a vector with the body moving commands for the l-system.
 */
void LSystem::build_moving_commands(){

    moving_commands.push_back("moveb"); // move back to parent in the turtle
    moving_commands.push_back("movel"); // move to the left in the turtle
    moving_commands.push_back("movef"); // move forward in the turtle
    moving_commands.push_back("mover"); // move to the right in the turtle

}


/**
 * Builds a vector with the body moving commands for the brain in the l-system.
 */
void LSystem::build_brainmove_commands(){


    // move fromid to parent N of fromid: brainmovep_N
    brainmove_commands.push_back("brainmovep");
    // move toid to child N of toid: brainmovec_N
    brainmove_commands.push_back("brainmovec");
}

/**
 * Builds a vector with commands for the l-system thatn change the brain.
 */
void LSystem::build_brainchange_commands(){

    // add link between nodes idfrom and idto with w: brainedge_w
    // if link already exists, add edge-node inbetween
    //brainchange_commands.push_back("brainedge");
    // perturb weight of connection between fromid and toid with: brainperturb_w
    brainchange_commands.push_back("brainperturb");
    // add new node between current fromid and toid with w from new to toid,
    // add self connection to fromid node with w: brainloop_w
    brainchange_commands.push_back("brainloop");

}

/**
 * Builds a vector with the possible letters of the alphabet of the l-system.
 */
void LSystem::build_alphabet(){


    alphabet["C"]  = "C";  // core component
    alphabet_index.push_back("C");
    alphabet_type["C"] = "body";

    alphabet["AJ1"] = "AJ1";     // active joint - horizontal (0 degrees)
    alphabet_index.push_back("AJ1");
    alphabet_type["AJ1"] = "body";

    alphabet["AJ2"] = "AJ2";     // active joint - vertical (90 degrees)
    alphabet_index.push_back("AJ2");
    alphabet_type["AJ2"] = "body";

    alphabet["B"] = "B";   // brick
    alphabet_index.push_back("B");
    alphabet_type["B"] = "body";

    alphabet["SI"] = "SI";   // infra-red sensor
    alphabet_index.push_back("SI");
    alphabet_type["SI"] = "sensor";

    alphabet["SL"] = "SL";   // light sensor
    alphabet_index.push_back("SL");
    alphabet_type["SL"] = "sensor";

}


std::map< std::string, std::string > LSystem::getAlphabet(){
    return this->alphabet;
};

std::vector<std::string> LSystem::getMountingCommands() {
    return this->mounting_commands;
};

std::vector<std::string> LSystem::getMovingCommands() {
    return this->moving_commands;
};

std::vector<std::string> LSystem::getBrainMoveCommands() {
    return this->brainmove_commands;
};

std::vector<std::string> LSystem::getBrainChangeCommands() {
    return this->brainchange_commands;
};

std::vector< std::string > LSystem::getAlphabetIndex(){
    return this->alphabet_index;
};

std::map< std::string, std::string > LSystem::getAlphabetType(){
    return this->alphabet_type;
};