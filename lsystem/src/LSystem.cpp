//
// Created by Karine Miras on 07/03/2017.
//

#include "LSystem.h"

/**
 *  Alphabet and commands
 *  < : back to parent on the parser build-tree
 *  addl : add component to the left side of the parent-reference
 *  addf : add component to the front side of the parent-reference
 *  addr : add component to the right side of the parent-reference
 *  CNNN : core component
 *  J1, J3, J3: 3 different flavors of fixed joint, with angles to be evolved
 *  AJ : active hinge joint
 *  PJ: passive hinge joint
 *  B--- : brick component with possibility of sensors I (infra-red) or L (light) or N (none)
 *         on the sides --- left/front/right.
 *         Ex.: BLIN means brick with light sensor on the left, infra-red on the front, and no sensor on the right
 */

void LSystem::build_commands(){

    commands.push_back("<");
    commands.push_back("addl");
    commands.push_back("addf");
    commands.push_back("addr");
}



void LSystem::build_alphabet(){


    alphabet["CNNN"]  = "CNNN";  // core component
    alphabet["J1"] = "J1";  // fixed joint - horizontal                                                             f§
    alphabet["J2"] = "J2";  // fixed joint - vertical
    alphabet["AJ1"] = "AJ1"; // active joint - horizontal
    alphabet["AJ2"] = "AJ2"; // active joint - vertical
    alphabet["PJ1"] = "PJ1";  // passive joint - horizontal
    alphabet["PJ2"] = "PJ2";  // passive joint - vertical

    alphabet["BNNN"] = "BNNN";  // brick (no sensors)

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