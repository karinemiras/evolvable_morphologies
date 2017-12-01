//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "GeneticString.h"

/**
 * Add items to the genetic-string.
 * @param genetic_string_items - contains the items which will be added to the genetic-string in an specific position.
 */
void GeneticString::add(
        int pos,
        std::string genetic_string_item)
{
    GeneticString::Node *current = nullptr, *next = nullptr, *inode = nullptr, *previous = nullptr;
    current = start;
    int current_pos = 1;

    while (current_pos < pos)
    { // looks for chosen position, as items will be inserted after that position, unless pos is 0, in case which the items will be the first ones
        current = current->next;
        current_pos++;
    }

    if (pos > 0)
    { // if insertion will NOT be done at the beginning
        previous = current;
        next = current->next;
    }
    else
    { // if insertion will be done at the beginning

        previous = nullptr;
        next = current;
    }

    // the new item
    inode = new GeneticString::Node();
    inode->item = genetic_string_item;
    inode->prev = previous;
    inode->next = next;

    if (previous != nullptr)
    {
        previous->next = inode;  // the item that comes before the chosen position points forward to the new one
    }
    else
    {
        start = inode; // in case the added item should become the first
    }

    if (next != nullptr)
    { // the item that comes after the chosen position points back to the new one
        next->prev = inode;
    }
}

/**
 * Joins parts of genetic-strings of two parents together.
 * @param gs_parent1 - genetic-string of parent1
 * @param gs_parent2 - genetic-string of parent2
 */
void GeneticString::create_joined_list(
        int pos_parent1_ini,
        int pos_parent2_ini,
        int pos_parent1_end,
        int pos_parent2_end,
        GeneticString  gs_parent1,
        GeneticString  gs_parent2)
{
    GeneticString::Node *current, *inode, *current_parent1, *current_parent2;

    int pos_parent1 = 1;
    int pos_parent2 = 1;

    current = start; // genetic-string of the offspring
    current_parent1 = gs_parent1.start; // genetic-string of the parent1
    current_parent2 = gs_parent2.start; // genetic-string of the parent2

    while (pos_parent1 <= pos_parent1_end)
    {
        if (pos_parent1 >= pos_parent1_ini)
        {
            inode = new GeneticString::Node(); // new item, copied from parent 1
            inode->item = current_parent1->item;
            inode->prev = current;
            inode->next = nullptr;

            if (current != nullptr)
            {
                current->next = inode;
            }
            current = inode; // new item becomes the current

            if (start == nullptr)
            {
                start = current; // if it is the first element of the genetic-string
            }
        }
        current_parent1 = current_parent1->next;

        pos_parent1++;
    }

    while (pos_parent2 <= pos_parent2_end)
    {
        if (pos_parent2 >= pos_parent2_ini)
        {
            inode = new GeneticString::Node(); // new item, copied from parent 2
            inode->item = current_parent2->item;
            inode->prev = current;
            inode->next = nullptr;

            if (current != nullptr)
            {
                current->next = inode;
            }
            current = inode; // new item becomes the current

            if (start == nullptr)
            {
                start = current; // if it is the first element of the genetic-string
            }
        }
        current_parent2 = current_parent2->next;

        pos_parent2++;
    }
}

/**
 * Deletes item from the genetic-string.
 * @param pos - position of item to be deleted
 */
void GeneticString::remove(int pos)
{
    GeneticString::Node *current = nullptr, *next = nullptr, *previous = nullptr;
    current = start;
    int current_pos = 1;

    while (current_pos < pos)
    { // looks for chosen position
        current = current->next;
        current_pos++;
    }

    previous = current->prev;
    next = current->next;

    if (previous != nullptr)
    {
        previous->next = next;  // that item that comes before the deleted item will point forward to the next item
    }
    else
    {
        start = next; // in case the removed item is the first, the next will be the first
    }

    if (next != nullptr)
    {
        next->prev = previous;  // the item that was positioned after the removed item will point to the new item
    }
}


/**
 * Swaps item from positions in the genetic-string.
 * @param pos_swap1 - position for item 1 to be swapped
 * @param pos_swap2 - position for item 2 to be swapped
 */
void GeneticString::swap(
        int pos_swap1,
        int pos_swap2)
{
    GeneticString::Node *current, *pos1;
    current = start;
    std::string aux = "";

    int current_pos = 1;

    while (current_pos < pos_swap1)
    { // looks for chosen position1
        current = current->next;
        current_pos++;
    }
    pos1 = current;

    while (current_pos < pos_swap2)
    { // looks for chosen position2
        current = current->next;
        current_pos++;
    }
    aux = current->item;

    // inverts values
    current->item = pos1->item;
    pos1->item = aux;
}

/**
 * Performs replacements in the genetic-string given the production rules of the grammar.
 * @param grammar - contains the production rules for each letter of the alphabet.
 */
void GeneticString::replaces(std::map< std::string, GeneticString  > grammar)
{
    if (start == nullptr)
    {
        std::cout << "WARNING: List empty, nothing to replace." << std::endl;
        return;
    }

    GeneticString::Node *current, *previous, *next, *inode, *current_replacement;
    current = start;

    while (current not_eq nullptr)
    { // for each letter on the main genetic-string of the genome, performs the due replacements (remotion/insertions) with other letters/commands
        // if there are brain parameters for the letter (joint)
        std::vector<std::string> tokens;
        boost::split(tokens, current->item, boost::is_any_of("_"));
        std::string aux_item;
        if(tokens.size()>1) aux_item = tokens[0];
        else aux_item = current->item;

        if (!(grammar.find(aux_item) == grammar.end()))
        {
            current_replacement = grammar[aux_item].start; // pointer to
            // the first item of the genetic-string of the production rule for the letter (item)

            // removes letter from the main genetic-string, once it will be replaced by other item/s
            previous = current->prev;
            next = current->next;

            while (current_replacement not_eq nullptr)
            {
                // the new item takes the place of the removed item
                inode = new GeneticString::Node();
                inode->item = current_replacement->item;
                inode->prev = previous;
                inode->next = next;

                if (previous != nullptr)
                {
                    previous->next = inode;  // the item that was positioned before the removed item will point to the new item
                }
                else
                {
                    start = inode; // in case the removed item was the first
                }
                if (next != nullptr)
                {
                    next->prev = inode;  // the item that was positioned after the removed item will point to the new item
                }

                previous = inode; // moves forward in the main genetic-string, ahead the just-added item
                next = inode->next;

                current_replacement = current_replacement->next; // moves forward in the genetic-string of the production rule
            }
        }
        current = current->next;
    }
}

/**
 * Creates a doubly-linked list representing a piece of genetic-string.
 * @param genetic_string_items - a vector containing all items (letters/commands) which will constitute the genetic-string to be built.
 */
void GeneticString::create_list(std::vector< std::string > genetic_string_items)
{
    if (start == nullptr)
    {
        Node *current = nullptr, *inode = nullptr;

        for (int i = 0; i < genetic_string_items.size(); i++)
        {   // adds each item of the vector in the list (genetic-string)

            inode = new GeneticString::Node(); // creates new node for the new
            // item
            inode->item = genetic_string_items[i];

            if (i == 0)
            { // initializes the list with its first item
                inode->prev = nullptr;
                inode->next = nullptr;
                current = inode;
                start = current;
            }
            else
            { // includes new item t the end of the list
                current->next = inode;
                inode->prev = current;
                current = inode;
            }
        }
    }
    else
    {
        std::cout << "WARNING: List has been created already." << std::endl;
        return;
    }
}

/**
 * Display elements of the genetic-string.
 */
void GeneticString::display_list()
{
    Node *current;
    if (start == nullptr)
    {
        std::cout << "WARNING: List empty, nothing to display." << std::endl;
        return;
    }
    current = start;
    std::cout << "The genetic-string List is :" << std::endl;
    while (current != nullptr)
    {
        std::cout << current->item << " ";
        current = current->next;
    }

    std::cout << std::endl << std::endl;
}

/**
 * Counts the of elements in the genetic-string.
 */
int GeneticString::count()
{
    Node *current = start;
    int cnt = 0;
    while (current != nullptr)
    {
        current = current->next;
        cnt++;
    }
    return cnt;
}

/**
 * @return pointer to the first item of the genetic-string
 */
GeneticString::Node *GeneticString::getStart()
{
    return this->start;
}
