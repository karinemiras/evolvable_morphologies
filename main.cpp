/*
 * Genenome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 * Edited: 03/03/17
 */

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <map>

using namespace std;



/*
 * Structure representing the itens of the genetic-string for each genome.
 */

class genetic_string
{
    // item of the genetic-string (doubly-linked list)
    struct node
    {
        string item;
        struct node *next;
        struct node *prev;
    }*start;

public:
    void create_list(vector<string> genetic_string_items);
    void display_list();
    void count();
    void replaces(map< string, genetic_string >  grammar);
    genetic_string()
    {
        start = NULL;
    }
};


/*
 *  Genome of an individual.
 */
class genome{


public:

    genetic_string gs;
    std::vector<string> axiom;
    map< string, genetic_string >  grammar;

    genetic_string build_genetic_string(genetic_string gs, std::vector<string> genetic_string_items);
    void build_grammar();
    genome(std::vector<string> paxiom){
        axiom = paxiom;
    }

};

// builds a piece of genetic-string for a genome with the given items
genetic_string genome::build_genetic_string(genetic_string gs, std::vector<string> genetic_string_items){

    try {
        gs.create_list(genetic_string_items);
        return gs;

    } catch (const std::exception& e) {
        cout <<"ERROR building axiom: " << e.what() << endl;

    }
}




int main()
{

    std::vector<string> axiom;
    axiom.push_back("B1");
    axiom.push_back("B2");
    axiom.push_back("B3");
    axiom.push_back("B4");

    std::vector<string> letter1_items;
    letter1_items.push_back("B1");
    letter1_items.push_back("B5");

    std::vector<string> letter2_items;
    letter2_items.push_back("B2");
    letter2_items.push_back("B6");

    genome gen(axiom);
    gen.gs = gen.build_genetic_string(gen.gs, gen.axiom);
    cout << "string original"<<endl;
    gen.gs.display_list();

    genetic_string gs_letter1;
    gs_letter1 = gen.build_genetic_string(gs_letter1,letter1_items);
    cout << "letter replacement 1"<<endl;
    gs_letter1.display_list();

    genetic_string gs_letter2;
    gs_letter2 = gen.build_genetic_string(gs_letter2,letter2_items);
    cout << "letter replacement 4"<<endl;
    gs_letter2.display_list();

    map< string, genetic_string >  grammar;
    grammar[ "B1" ] = gs_letter1;
    grammar[ "B4" ] = gs_letter2;

    for(int i=0;i<3;i++) {
        gen.gs.replaces(grammar);
        cout << "string iterastion " << i << endl;
        gen.gs.display_list();
    }


    return 0;
}



/*
 * Performs replecements in the genetic-string, giving the defined grammar and its production rules.
 */
void genetic_string::replaces(map< string, genetic_string >  grammar)
{
    if (start == NULL)
    {
        cout<<"WARNING: List empty, nothing to replace."<<endl;
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

/*
 * Creates a doubly-linked list representing a piece of genetic-string.
 */
void genetic_string::create_list(vector<string> genetic_string_items)
{

    if (start == NULL)
    {
        struct node *current,*inode;

        // adds each item of the axiom in the list
        for (int i=0; i< genetic_string_items.size();i++) {

            inode = new(struct node);
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
        cout<<"WARNING: List has been created already."<<endl;
        return;
    }
}


/*
 * Display elements of the List
 */
void genetic_string::display_list()
{
    struct node *current;
    if (start == NULL)
    {
        cout<<"WARNING: List empty,nothing to display"<<endl;
        return;
    }
    current = start;
    cout<<"The genetic-string List is :"<<endl;
    while (current != NULL)
    {
        cout<<current->item<<" >-< ";
        current = current->next;
    }
    cout<<endl<<endl;
}

/*
 * Number of elements in the List
 */
void genetic_string::count()
{
    struct node *current = start;
    int cnt = 0;
    while (current != NULL)
    {
        current = current->next;
        cnt++;
    }
    cout<<"Number of elements are: "<<cnt<<endl;
}

