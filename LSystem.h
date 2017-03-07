//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_LSYSTEM_H
#define LSYSTEM_PROTO_LSYSTEM_H

#include <string>
#include <vector>

/*
 * Alphabet containing the letters and the commands com the l-system.
 */
class LSystem{

public:
    void build_commands();
    void build_alphabet();
    std::vector<std::string> getAlphabet();
    std::vector<std::string> getCommands();

private:
    std::vector<std::string> commands;
    std::vector<std::string> alphabet;

};


#endif //LSYSTEM_PROTO_LSYSTEM_H
