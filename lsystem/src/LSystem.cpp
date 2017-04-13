//
// Created by Karine Miras on 07/03/2017.
//

#include "LSystem.h"


/**
 * Builds a vector with the possible mounting commands for the l-system.
 */
void LSystem::build_commands(){

    commands.push_back("<");    //  back-to-parent on the parser build-tree
    commands.push_back("addl"); // add component to the left side of the parent-reference
    commands.push_back("addf"); //  add component to the front side of the parent-reference
    commands.push_back("addr"); // add component to the right side of the parent-reference
}


/**
 * Builds a vector with the possible letters of the alphabet of the l-system.
 */
void LSystem::build_alphabet(){


    alphabet["CNNN"]  = "CNNN";  // core component (no sensors)
    alphabet["J1"] = "J1";       // fixed joint - horizontal                                                             f§
    alphabet["J2"] = "J2";       // fixed joint - vertical
    alphabet["AJ1"] = "AJ1";     // active joint - horizontal
    alphabet["AJ2"] = "AJ2";     // active joint - vertical
    alphabet["PJ1"] = "PJ1";     // passive joint - horizontal
    alphabet["PJ2"] = "PJ2";     // passive joint - vertical
    alphabet["BNNN"] = "BNNN";   // brick (no sensors)

 /**
 *  B--- : brick component with possibility of sensors I (infra-red) or L (light) or N (none)
 *         on the sides --- left/front/right.
 *         Ex.: BLIN means brick with light sensor on the left, infra-red on the front, and no sensor on the right
 */

//    std::vector<std::string> sensors;
//    sensors.push_back("L"); // light sensor
//    sensors.push_back("I"); // infra-red sensor
//    sensors.push_back("N"); // no sensor

//    for(int l=0; l<3 ;l++) { // for each type of sensor in the left side
//
//        for(int f=0; f<3 ;f++) {  // for each type of sensor in the front side
//
//            for(int r=0; r<3 ;r++) {  // for each type of sensor in the right side
//                std::string letter = "B"+sensors[l]+sensors[f]+sensors[r];
//                alphabet[letter] = letter;
//            }
//        }
//    }

    for (std::map< std::string, std::string >::const_iterator it = alphabet.begin(); it != alphabet.end(); ++it) {
        alphabet_index.push_back(it->first);
    }
}


std::map< std::string, std::string > LSystem::getAlphabet(){
    return this->alphabet;
};

std::vector<std::string> LSystem::getCommands(){
    return this->commands;
};

std::vector< std::string > LSystem::getAlphabetIndex(){
    return this->alphabet_index;
};