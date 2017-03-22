//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENETICSTRING_H
#define LSYSTEM_PROTO_GENETICSTRING_H


#include <map>
#include<string>
#include <vector>


/**
 * Structure representing the itens of the genetic-string for each genome.
 */
class GeneticString
{
public:

    struct Node
    {
        std::string item;
        Node *next;
        Node *prev;
        Node() { next=prev=NULL; }
    };

    void create_list(std::vector<std::string> genetic_string_items);
    void display_list();
    int count();
    void replaces(std::map< std::string, GeneticString >  grammar);
    GeneticString::Node * getStart();


    GeneticString()
    {
        start = NULL;
    }

private:

    Node *start; // item of the genetic-string (doubly-linked list)

};

#endif //LSYSTEM_PROTO_GENETICSTRING_H
