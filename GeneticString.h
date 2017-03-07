//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENETICSTRING_H
#define LSYSTEM_PROTO_GENETICSTRING_H


#include <map>
#include<string>
#include <vector>

/*
 * Structure representing the itens of the genetic-string for each genome.
 */
class GeneticString
{
    struct node
    {
        std::string item;
        struct node *next;
        struct node *prev;
    };


private:
    // item of the genetic-string (doubly-linked list)
    node *start = new node;

public:
    void create_list(std::vector<std::string> genetic_string_items);
    void display_list();
    void count();
    void replaces(std::map< std::string, GeneticString >  grammar);

    GeneticString()
    {
        start = NULL;
    }
};

#endif //LSYSTEM_PROTO_GENETICSTRING_H
