//
// Created by Karine Miras on 07/03/2017.
//
#include <random>

#include "LSystem.h"

/**
 * Transfer functions for the brain.
 */
void LSystem::build_brain_functions(){
    brainfunctions.push_back("Simple");
    brainfunctions.push_back("Sigmoid");
    brainfunctions.push_back("Oscillator");
}

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


    // change 'from' of current-edge to child of current-from
    brainmove_commands.push_back("brainmoveFTC");
    // change 'from' of current-edge to parent of current-from
    brainmove_commands.push_back("brainmoveFTP");
    // change 'from' of current-edge to sibling of current-from
    brainmove_commands.push_back("brainmoveFTS");
    // change 'to' of current-edge to child of current-to
    brainmove_commands.push_back("brainmoveTTC");
    // change 'to' of current-edge to child of current-to
    brainmove_commands.push_back("brainmoveTTP");
    // change 'to' of current-edge to sibling of current-to
    brainmove_commands.push_back("brainmoveTTS");
}

/**
 * Builds a vector with commands for the l-system thatn change the brain.
 */
void LSystem::build_brainchange_commands(){

    // add link between nodes idfrom and idto with w: brainedge_w
    // add node between nodes
    brainchange_commands.push_back("brainnode");
    // add edge between nodes
    brainchange_commands.push_back("brainedge");
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

    alphabet["ST"] = "ST";   // touch  sensor
    alphabet_index.push_back("ST");
    alphabet_type["ST"] = "sensor";

    //alphabet["SL"] = "SL";   // light sensor
    //alphabet_index.push_back("SL");
    //alphabet_type["SL"] = "sensor";

}


/**
 * Enhances brain commands with a parameter.
 **/
std::string LSystem::buildBrainCommand(std::string braincommand)
{
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> weight_uni(-1, 1);
    std::uniform_real_distribution<double> weight_uni2(1, 10);
    std::normal_distribution<double> weight_nor(0, 1);
    std::uniform_int_distribution<int> func(0, this->brainfunctions.size()-1);

    if(braincommand.substr(0,1) == "A")
    {
        // new connection weight
        braincommand += "_"+std::to_string(weight_uni(generator));
        // transfer/activation function
        auto function = this->brainfunctions[func(generator)];
        braincommand += "|" + function;

        if(function == "Oscillator")
        {
          // amplitude
          braincommand += "|" + std::to_string(weight_uni2(generator));
          // period
          braincommand += "|" + std::to_string(weight_uni2(generator));
          // phase_offset
          braincommand += "|" + std::to_string(weight_uni2(generator));
        }else
          // bias weight
          braincommand += "|"+std::to_string(weight_uni(generator));
    }

    if(braincommand.substr(0,1) == "S")
    {
      // new connection weight
      braincommand += "_"+std::to_string(weight_uni(generator));
    }

    if(   braincommand == "brainedge"
          or braincommand == "brainloop")
        // new connection weight
        braincommand += "_"+std::to_string(weight_uni(generator));

    if(  braincommand == "brainnode")
    {
        // new connection weight
        braincommand += "_" + std::to_string(weight_uni(generator));
        // transfer/activation function
        auto function = this->brainfunctions[func(generator)];
        braincommand += "|" + function;

        if(function == "Oscillator")
        {
          // amplitude
          braincommand += "|" + std::to_string(weight_uni2(generator));
          // period
          braincommand += "|" + std::to_string(weight_uni2(generator));
          // phase_offset
          braincommand += "|" + std::to_string(weight_uni2(generator));
        }else
          // bias weight
          braincommand += "|" + std::to_string(weight_uni(generator));
    }

    if(braincommand == "brainperturb")
        braincommand += "_"+std::to_string(weight_nor(generator));

    // its more likely that a node has a number of conn close to 1

    if(   braincommand == "brainmoveFTC"
          or  braincommand == "brainmoveFTP"
          or braincommand == "brainmoveTTC"
          or  braincommand == "brainmoveTTP")
    {
        double child = weight_nor(generator);
        child = ceil(sqrt(child*child));
        braincommand += "_"+std::to_string(child);
    }

    if(   braincommand == "brainmoveTTS"
          or  braincommand == "brainmoveFTS")
    {
        // intermediate node to roam
        double intermediate = weight_nor(generator);
        intermediate = ceil(sqrt(intermediate*intermediate));
        // sibbling node to roam
        double sibling = weight_nor(generator);
        sibling = ceil(sqrt(sibling*sibling));

        braincommand += "_"+std::to_string(intermediate)
                        +"|"+std::to_string(sibling);
    }

    return braincommand;

}




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

std::vector<std::string> LSystem::getBrainFunctions() {
    return this->brainfunctions;
};

std::vector< std::string > LSystem::getAlphabetIndex(){
    return this->alphabet_index;
};

std::map< std::string, std::string > LSystem::getAlphabetType(){
    return this->alphabet_type;
};

std::map< std::string, std::string > LSystem::getAlphabet(){
    return this->alphabet;
};

