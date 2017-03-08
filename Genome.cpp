//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>
#include <random>


#include "Genome.h"


void Genome::setGeneticString(GeneticString _gs){
    this->gs = _gs;
}

GeneticString Genome::getGeneticString(){
    return this->gs;
}

std::vector<std::string> Genome::getAxiom(){
    return this->axiom;
}

std::map< std::string, GeneticString > Genome::getGrammar(){
    return this->grammar;
}


// generates random production rules for the alphabet ### remove static grammar after test, and then use the dynamic one ###
void Genome::build_grammar(LSystem LS){

//    std::random_device rd;
//    std::default_random_engine re(rd());
//    int asize = LS.getAlphabet().size();
//    std::uniform_int_distribution<int> uniform_dist(1, asize);
//    std::vector<std::string> alp = LS.getAlphabet();
//
//    for(int l=0; l<asize; l++){
//
//        int pos = uniform_dist(re)-1;
//
//        std::vector<std::string> letter_items;
//        letter_items.push_back(alp[l]);
//        letter_items.push_back(alp[pos]);
//        GeneticString gs;
//        gs = build_genetic_string(gs,letter_items);
//        std::cout << "letter replacement "<<alp[l]<<std::endl;
//        gs.display_list();
//        this->grammar[ alp[l] ] = lgs;

 //   }

    std::vector<std::string> letter1_items;
    letter1_items.push_back("C");
    letter1_items.push_back("addr");
    letter1_items.push_back("B1");



    std::vector<std::string> letter2_items;
    letter2_items.push_back("B1");
    letter2_items.push_back("addr");
    letter2_items.push_back("B1");


    GeneticString gs_letter1;
    gs_letter1 = build_genetic_string(gs_letter1,letter1_items);
    std::cout << "letter replacement C"<<std::endl;
    gs_letter1.display_list();

    GeneticString gs_letter2;
    gs_letter2 = build_genetic_string(gs_letter2,letter2_items);
    std::cout << "letter replacement B1"<<std::endl;
    gs_letter2.display_list();

    this->grammar[ "C" ] = gs_letter1;
    this->grammar[ "B1" ] = gs_letter2;


}

// performs production rules for a number of iterations
void Genome::generate_final_string(){

    int iterations = 2;

    for(int i=0;i<iterations;i++) {
       this->gs.replaces(this->grammar);
       std::cout << "string iteration " << i << std::endl;
        this->gs.display_list();
    }

}