//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>

#include "GeneticString.h"



/**
 * Add items to the genetic-string.
 * @param genetic_string_items - contains the items which will be added to the genetic-string in an specific position.
 */
void GeneticString::alter(int pos, std::vector<std::string> genetic_string_items) {

    GeneticString::Node *current, *next, *inode, *previous;
    current = start;
    int current_pos = 0;

    while (current_pos != pos) { // looks for chosen position, as items will be inserted after that position
        current = current->next;
        current_pos++;
    }

    previous = current;
    next = current->next;

    for (int i = 0; i < genetic_string_items.size(); i++) {

        // the new item
        inode = new GeneticString::Node;
        inode->item = genetic_string_items[i];
        inode->prev = previous;
        inode->next = next;

        previous->next = inode;

        if (next != NULL) {
            next->prev = inode;
        }

        previous = inode;
        next = inode->next;
    }
}

/**
 * Performs replacements in the genetic-string given the production rules of the grammar.
 * @param grammar - contains the production rules for each letter of the alphabet.
 */
void GeneticString::replaces(std::map< std::string, GeneticString >  grammar){

    if (start == NULL)
    {
        std::cout<<"WARNING: List empty, nothing to replace."<<std::endl;
        return;
    }

    GeneticString::Node *current, *previous, *next, *inode, *current_replacement;
    current = start;

    while (current != NULL){ // for each letter on the main genetic-string of the genome, performs the due replacements (remotion/insertions) with other letters/commands

        if (!(grammar.find(current->item) == grammar.end() )){ // checks if the item is a letter of the grammar

            current_replacement = grammar[current->item].start; // pointer to the first item of the genetic-string of the production rule for the letter (item)

            // removes letter from the main genetic-string, once it will be replaced by other item/s
            previous = current->prev;
            next = current->next;

            while(current_replacement != NULL) { // inserts the items of the genetic-string of the production rule to the main genetic-string

                // the new item takes the place of the removed item
                inode = new GeneticString::Node;
                inode->item = current_replacement->item;
                inode->prev = previous;
                inode->next = next;

                if (previous != NULL) {
                    previous->next = inode;  // the item that was positioned before the removed item will point to the new item
                }else{
                    start = inode; // in case the removed item was the first
                }
                if (next != NULL) {
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
void GeneticString::create_list(std::vector<std::string> genetic_string_items)
{

    if (start == NULL) {
        Node *current = NULL,*inode = NULL;

        for (int i=0; i< genetic_string_items.size();i++) {   // adds each item of the vector in the list (genetic-string)

            inode = new Node; // creates new node for the new item
            inode->item = genetic_string_items[i];

            if (i==0) { // initializes the list with its first item
                inode->prev = NULL;
                inode->next = NULL;
                current = inode;
                start = current;
            }else { // includes new item t the end of the list
                current->next = inode;
                inode->prev = current;
                current = inode;
            }
        }

    }
    else {
        std::cout<<"WARNING: List has been created already."<<std::endl;
        return;
    }
}


/**
 * Display elements of the genetic-string.
 */
void GeneticString::display_list()
{
    Node *current;
    if (start == NULL)
    {
        std::cout<<"WARNING: List empty, nothing to display."<<std::endl;
        return;
    }
    current = start;
    std::cout<<"The genetic-string List is :"<<std::endl;
    while (current != NULL) {
        std::cout<<current->item<<" ";
        current = current->next;
    }

    std::cout<<std::endl<<std::endl;
}

/**
 * Counts the of elements in the genetic-string.
 */
    int GeneticString::count()
{
    Node *current = start;
    int cnt = 0;
    while (current != NULL)
    {
        current = current->next;
        cnt++;
    }
    return cnt;
}


/**
 * @return pointer to the first item of the genetic-string
 */
GeneticString::Node * GeneticString::getStart(){
    return this->start;
}
