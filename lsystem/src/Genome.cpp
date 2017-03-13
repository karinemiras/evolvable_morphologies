//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>
#include <random>
#include <string>

#include "Genome.h"

unsigned int Genome::getTo() {
    return 3;
}

void Genome::setGeneticString(GeneticString _gs){
    this->gs = _gs;
}

GeneticString Genome::getGeneticString(){
    return this->gs;
}

std::vector<std::string> Genome::getAxiom(){
    return this->axiom;
}



/**
* Generates initial production rules for the alphabet
*/
void Genome::build_grammar(LSystem LS){

    std::map< std::string, std::string > alp = LS.getAlphabet();

//    for (std::map< std::string, std::string >::const_iterator it = alp.begin(); it != alp.end(); ++it) {
//
//        std::vector<std::string> letter_items;
//        letter_items.push_back(it->first);
//        GeneticString lgs;
//        lgs = build_genetic_string(lgs,letter_items);
//        lgs.display_list();
//        this->grammar[ it->first ] = lgs;
//    }

    // manual production 3
    std::vector<std::string> letter_c;
    letter_c.push_back("CNNN");
    letter_c.push_back("addr");
    letter_c.push_back("BNNN");
    GeneticString cgs;
    cgs = build_genetic_string(cgs,letter_c);
    cgs.display_list();
    this->grammar[ "CNNN" ] = cgs;

    std::vector<std::string> letter_b;
    letter_b.push_back("BNNN");
    letter_b.push_back("<");
    letter_b.push_back("<");
    letter_b.push_back("addl");
    letter_b.push_back("J1");
    GeneticString bgs;
    bgs = build_genetic_string(bgs,letter_b);
    bgs.display_list();
    this->grammar[ "BNNN" ] = bgs;

    std::vector<std::string> letter_j;
    letter_j.push_back("addf");
    letter_j.push_back("J1");
    letter_j.push_back("addf");
    letter_j.push_back("BNNN");
    letter_j.push_back("<");
    letter_j.push_back("<");
    letter_j.push_back("addl");
    letter_j.push_back("BNNN");
    GeneticString jgs;
    jgs = build_genetic_string(jgs,letter_j);
    jgs.display_list();
    this->grammar[ "J1" ] = jgs;

    // manual production 1
//    std::vector<std::string> letter_c;
//    letter_c.push_back("CNNN");
//    letter_c.push_back("addr");
//    letter_c.push_back("BNNN");
//    GeneticString cgs;
//    cgs = build_genetic_string(cgs,letter_c);
//    cgs.display_list();
//    this->grammar[ "CNNN" ] = cgs;
//
//    std::vector<std::string> letter_b;
//    letter_b.push_back("addl");
//    letter_b.push_back("J1");
//    letter_b.push_back("<");
//    letter_b.push_back("addr");
//    letter_b.push_back("J1");
//    GeneticString bgs;
//    bgs = build_genetic_string(bgs,letter_b);
//    bgs.display_list();
//    this->grammar[ "BNNN" ] = bgs;
//
//    std::vector<std::string> letter_j;
//    letter_j.push_back("J1");
//    letter_j.push_back("addf");
//    letter_j.push_back("BNNN");
//    letter_j.push_back("<");
//    letter_j.push_back("<");
//    GeneticString jgs;
//    jgs = build_genetic_string(jgs,letter_j);
//    jgs.display_list();
//    this->grammar[ "J1" ] = jgs;

    // manual production 2
//    std::vector<std::string> letter_c;
//    letter_c.push_back("CNNN");
//    letter_c.push_back("addr");
//    letter_c.push_back("BNNN");
//    GeneticString cgs;
//    cgs = build_genetic_string(cgs,letter_c);
//    cgs.display_list();
//    this->grammar[ "CNNN" ] = cgs;
//
//    std::vector<std::string> letter_b;
//    letter_b.push_back("addl");
//    letter_b.push_back("J1");
//    letter_b.push_back("<");
//    letter_b.push_back("addr");
//    letter_b.push_back("J1");
//    GeneticString bgs;
//    bgs = build_genetic_string(bgs,letter_b);
//    bgs.display_list();
//    this->grammar[ "BNNN" ] = bgs;
//
//    std::vector<std::string> letter_j;
//    letter_j.push_back("J1");
//    letter_j.push_back("addf");
//    letter_j.push_back("BNNN");
//    letter_j.push_back("addf");
//    letter_j.push_back("AJ");
//    letter_j.push_back("addf");
//    letter_j.push_back("BNNN");
//    letter_j.push_back("<");
//    letter_j.push_back("<");
//    letter_j.push_back("<");
//    letter_j.push_back("<");
//    GeneticString jgs;
//    jgs = build_genetic_string(jgs,letter_j);
//    jgs.display_list();
//    this->grammar[ "J1" ] = jgs;



//    std::vector<std::string> letter_p;
//    letter_p.push_back("PJ");
//    GeneticString pgs;
//    pgs = build_genetic_string(pgs,letter_p);
//    pgs.display_list();
//    this->grammar[ "PJ" ] = pgs;
//
//    std::vector<std::string> letter_a;
//    letter_a.push_back("AJ");
//    GeneticString ags;
//    ags = build_genetic_string(ags,letter_a);
//    ags.display_list();
//    this->grammar[ "AJ" ] = ags;
}

/**
 * Performs production rules for a number of iterations.
 */
void Genome::generate_final_string(){

    int iterations = 3;

    for(int i=0;i<iterations;i++) {
        this->gs.replaces(this->grammar);
        std::cout << "string iteration " << i << std::endl;
        this->gs.display_list();
    }

}


/**
* Builds a piece of genetic-string for a genome with the given items.
 */
GeneticString Genome::build_genetic_string(GeneticString gs, std::vector<std::string> genetic_string_items){

    try {
        gs.create_list(genetic_string_items);
        return gs;

    } catch (const std::exception& e) {
        std::cout <<"ERROR building axiom: " << e.what() << std::endl;

    }
}


/**
 *  Decodes the genetic-string into a tree of components.
 */
void Genome::decodeGeneticString(LSystem LS){

    try {
        this->dgs.decode(this->gs,LS);

    } catch (const std::exception& e) {
        std::cout <<"ERROR decoding genetic-string: " << e.what() << std::endl;

    }

}

