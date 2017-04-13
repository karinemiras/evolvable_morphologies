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
        void build_commands();
        void build_alphabet();
        std::map< std::string, std::string > getAlphabet();
        std::vector<std::string> getAlphabetIndex();
        std::vector<std::string> getCommands();


    private:
        std::vector<std::string> commands = std::vector<std::string>(); // vector with the commands of the l-system
        std::map< std::string, std::string > alphabet = std::map< std::string, std::string >();  // alphabet with the letters of the l-system
        std::vector<std::string> alphabet_index = std::vector<std::string>(); // auxiliar vector for the alphabet letters
};


#endif //LSYSTEM_PROTO_LSYSTEM_H
