//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_LSYSTEM_H
#define LSYSTEM_PROTO_LSYSTEM_H

#include<map>
#include <string>
#include <vector>


/**
 * Alphabet containing the letters and the commands com the l-system.
 */
class LSystem{

public:
    void build_moving_commands();
    void build_mounting_commands();
    void build_brainmove_commands();
    void build_brainchange_commands();
    void build_brain_functions();
    void build_alphabet();
    std::map< std::string, std::string > getAlphabet();
    std::vector<std::string> getAlphabetIndex();
    std::map< std::string, std::string > getAlphabetType();
    std::vector<std::string> getMovingCommands();
    std::vector<std::string> getMountingCommands();
    std::vector<std::string> getBrainMoveCommands();
    std::vector<std::string> getBrainFunctions();
    std::vector<std::string> getBrainChangeCommands();
    std::string buildBrainCommand(std::string braincommand);

    LSystem(){
        build_moving_commands();
        build_mounting_commands();
        build_brainmove_commands();
        build_brainchange_commands();
        build_brain_functions();
        build_alphabet();
    };

private:
    // vector with the commands to move in the turtle
    std::vector<std::string> moving_commands = std::vector<std::string>();
    // vector with the commands to mount components in the turtle
    std::vector<std::string> mounting_commands = std::vector<std::string>();
    // vector with the commands to move in the brain
    std::vector<std::string> brainmove_commands = std::vector<std::string>();
    // vector with the commands to change the brain
    std::vector<std::string> brainchange_commands = std::vector<std::string>();
    // alphabet with the letters
    std::map< std::string, std::string > alphabet = std::map<std::string, std::string >();
    // vector with functions for the brain
    std::vector<std::string> brainfunctions = std::vector<std::string>();
    // auxiliar vector for the alphabet letters
    std::vector<std::string> alphabet_index = std::vector<std::string>();
    // auxiliar vector for type of items in the alphabet
    std::map< std::string, std::string > alphabet_type = std::map<std::string,
            std::string >();
};


#endif //LSYSTEM_PROTO_LSYSTEM_H