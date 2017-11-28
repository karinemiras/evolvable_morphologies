//
// Created by Karine Miras on 07/03/2017.
//

#ifndef LSYSTEM_PROTO_GENETICSTRING_H
#define LSYSTEM_PROTO_GENETICSTRING_H


#include <map>
#include <string>
#include <vector>


/**
 * Structure of the items composing the first developmental stage a genome.
 * The structure is a list of which the items are letters and commands, which
 * belong to the alphabet.
 */
class GeneticString
{

public:
  GeneticString()
      : start(nullptr)
  {
  }

  ~GeneticString()
  {}

  // Structure representing the items of the genetic-strings (lists).
  struct Node
  {
    std::string item; // string that represents the item
    Node *next;
    Node *prev;

    Node()
    {
      item = "";
      next = prev = nullptr;
    }
  };

  void create_list(std::vector< std::string > genetic_string_items);

  void display_list();

  int count();

  void replaces(std::map< std::string, GeneticString  > grammar);

  GeneticString::Node *getStart();

  void add(
      int pos,
      std::string genetic_string_item);

  void remove(int pos);

  void create_joined_list(
      int pos_parent1_ini,
      int pos_parent2_ini,
      int pos_parent1_end,
      int pos_parent2_end,
      GeneticString  gs_parent1,
      GeneticString  gs_parent2);

  void swap(
      int pos_swap1,
      int pos_swap2);



private:

  Node *start; // item of the genetic-string (doubly-linked list)

};

#endif //LSYSTEM_PROTO_GENETICSTRING_H
