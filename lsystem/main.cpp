/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 * Edited: 03/03/17
 */


#include<cstdio>
#include<cstdlib>
#include<iostream>
#include <map>
#include <sstream>
#include <vector>



#include "Genome.h"
#include "LSystem.h"

using namespace std;


int main(int argc, char* argv[])
{

    /**
     *  Alphabet and commands
     *  < : back to parent on the parser build-tree
     *  addl : add component to the left side of the parent-reference
     *  addf : add component to the front side of the parent-reference
     *  addr : add component to the right side of the parent-reference
     *  C : core component
     *  J1, J3, J3: 3 different flavors of fixed joint, with angles to be evolved
     *  AJ : active hinge joint
     *  PJ: passive hinge joint
     *  B--- : brick component with possibility of sensors I (infra-red) or L (light) or N (none)
     *         on the sides --- left/front/right.
     *         Ex.: BLIN means brick with light sensor on the left, infra-red on the front, and no sensor on the right
     */

    LSystem LS;
    LS.build_commands();
    LS.build_alphabet();

    std::vector<string> axiom;
    axiom.push_back("CNNN");

    Genome gen(axiom);
    // initializes the genetic-string with the axiom
    gen.setGeneticString(gen.build_genetic_string(gen.getGeneticString(), gen.getAxiom()));

    cout << "string original:"<<endl;
    gen.getGeneticString().display_list();

    // creates genetic-strings for production rules of the grammar
    cout << "grammar:"<<endl;
    gen.build_grammar(LS);

    // enhances the genetic-string according to grammar iteratively
    gen.generate_final_string();

    // decodes the final genetic-string into a tree of components
    gen.decodeGeneticString(LS);

    // generates robot-graphics
    gen.constructor(argc, argv);

    return 0;

}


