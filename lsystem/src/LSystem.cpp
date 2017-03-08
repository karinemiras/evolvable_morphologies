//
// Created by Karine Miras on 07/03/2017.
//

#include "LSystem.h"

void LSystem::build_commands(){

    commands.push_back("<");
    commands.push_back(">");
    commands.push_back("addl");
    commands.push_back("addf");
    commands.push_back("addr");
}


void LSystem::build_alphabet(){

    alphabet.push_back("C");
    alphabet.push_back("J1");
    alphabet.push_back("J2");
    alphabet.push_back("J3");
    alphabet.push_back("AJ");
    alphabet.push_back("PJ");

    std::vector<std::string> sensors;
    sensors.push_back("L"); // light sensor
    sensors.push_back("I"); // infra-red sensor
    sensors.push_back("N"); // no sensor

    for(int l=0; l<3 ;l++) { // for each type of sensor in the left side

        for(int f=0; f<3 ;f++) {  // for each type of sensor in the front side

            for(int r=0; r<3 ;r++) {  // for each type of sensor in the right side
                std::string letter = "B"+sensors[l]+sensors[f]+sensors[r];
                alphabet.push_back(letter);
            }
        }
    }
}


std::vector<std::string> LSystem::getAlphabet() {
    return this->alphabet;
}

std::vector<std::string> LSystem::getCommands() {
    return this->commands;
}