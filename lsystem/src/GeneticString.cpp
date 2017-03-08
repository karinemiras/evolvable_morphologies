//
// Created by Karine Miras on 07/03/2017.
//

#include "GeneticString.h"

#include <iostream>


/**
 * Performs replacements in the genetic-string, giving the defined grammar and its production rules.
 */
void GeneticString::replaces(std::map< std::string, GeneticString >  grammar)
{
    if (start == NULL)
    {
        std::cout<<"WARNING: List empty, nothing to replace."<<std::endl;
        return;
    }

    struct node *current, *previous, *next, *inode, *current_rep;
    current = start;

    // for each item on the genetic string
    while (current != NULL)
    {

        // if this item is part of the grammar and has production rules
        if (!(grammar.find(current->item) == grammar.end() )){

            current_rep = grammar[current->item].start;

            // removes item from the genetic-string list
            previous = current->prev;
            next = current->next;

            if (previous != NULL) {
                previous->next = next;
            }else{
                start = next;
            }
            if (next != NULL) {
                next->prev = previous;
            }

            // for each item on the production string, adds new items to the list replacing the removed one
            while(current_rep != NULL) {

                inode = new(struct node);
                inode->item = current_rep->item;
                inode->prev = previous;
                inode->next = next;

                if (previous != NULL) {
                    previous->next = inode;
                }else{
                    start = inode;
                }
                if (next != NULL) {
                    next->prev = inode;
                }

                previous = inode;
                next = inode->next;

                current_rep = current_rep->next;
            }
        }
        current = current->next;
    }
}

/**
 * Creates a doubly-linked list representing a piece of genetic-string.
 */
void GeneticString::create_list(std::vector<std::string> genetic_string_items)
{

    if (start == NULL)
    {
        node *current,*inode;

        // adds each item of the axiom in the list
        for (int i=0; i< genetic_string_items.size();i++) {

            inode = new node;
            inode->item = genetic_string_items[i];

            if (i==0)
            {
                inode->prev = NULL;
                inode->next = NULL;
                current = inode;
                start = current;
            }else
            {
                current->next = inode;
                inode->prev = current;
                current = inode;
            }
        }

    }
    else
    {
        std::cout<<"WARNING: List has been created already."<<std::endl;
        return;
    }
}


/**
 * Display elements of the List
 */
void GeneticString::display_list()
{
    struct node *current;
    if (start == NULL)
    {
        std::cout<<"WARNING: List empty,nothing to display"<<std::endl;
        return;
    }
    current = start;
    std::cout<<"The genetic-string List is :"<<std::endl;
    while (current != NULL)
    {
        std::cout<<current->item<<" ";
        current = current->next;
    }
    std::cout<<std::endl<<std::endl;
}

/**
 * Number of elements in the List
 */
int GeneticString::count()
{
    struct node *current = start;
    int cnt = 0;
    while (current != NULL)
    {
        current = current->next;
        cnt++;
    }
    return cnt;
}

GeneticString::node * GeneticString::getStart(){
    return this->start;

}
