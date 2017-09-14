//
// Created by Karine Miras on 07/03/2017.
//

#include "LSystem.h"


/**
 * Builds a vector with the possible mounting commands for the l-system.
 */
void LSystem::build_mounting_commands(){

    mounting_commands.push_back("addl"); // add component to the left side of the parent-reference
    mounting_commands.push_back("addf"); //  add component to the front side of the parent-reference
    mounting_commands.push_back("addr"); // add component to the right side of the parent-reference
}

/**
 * Builds a vector with the possible mounting commands for the l-system.
 */
void LSystem::build_moving_commands(){

    moving_commands.push_back("moveb"); // move back to parent in the turtle
    moving_commands.push_back("movel"); // move to the left in the turtle
    moving_commands.push_back("movef"); // move forward in the turtle
    moving_commands.push_back("mover"); // move to the right in the turtle

}

/**
 * Builds a vector with the possible letters of the alphabet of the l-system.
 */
void LSystem::build_alphabet(){


    alphabet["C"]  = "C";  // core component
    alphabet_index.push_back("C");

    //alphabet["J1"] = "J";       // fixed joint - horizontal
    //alphabet_index.push_back("J1");

    alphabet["J2"] = "J2";       // fixed joint - vertical
    alphabet_index.push_back("J2");

    //alphabet["AJ1"] = "AJ1";     // active joint - horizontal
    //alphabet_index.push_back("AJ1");

    alphabet["AJ2"] = "AJ2";     // active joint - vertical
    alphabet_index.push_back("AJ2");

    //alphabet["PJ1"] = "PJ1";     // passive joint - horizontal
    //alphabet_index.push_back("PJ1");

    alphabet["PJ2"] = "PJ2";     // passive joint - vertical
    alphabet_index.push_back("PJ2");

    alphabet["B"] = "B";   // brick
    alphabet_index.push_back("B");

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

std::vector< std::string > LSystem::getAlphabetIndex(){
    return this->alphabet_index;
};