/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 * Edited: 03/03/17
 */


#include<cstdio>
#include<cstdlib>
#include<iostream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "Genome.h"
#include "LSystem.h"

using namespace std;


int main()
{

    /*
     *  Alphabet and commands
     *  > : branch out the parent on the parser build-tree
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
    axiom.push_back("C");


    Genome gen(axiom);
    gen.setGeneticString(gen.build_genetic_string(gen.getGeneticString(), gen.getAxiom()));

    cout << "string original"<<endl;
    gen.getGeneticString().display_list();

    gen.build_grammar(LS);

    gen.generate_final_string();



    return 0;
}


