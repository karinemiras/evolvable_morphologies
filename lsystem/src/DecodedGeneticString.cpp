//
// Created by Karine Miras on 08/03/2017.
//

#include "DecodedGeneticString.h"

#include <iostream>


DecodedGeneticString::node * DecodedGeneticString::getRoot(){
    return this->root;
}


void DecodedGeneticString::decode(GeneticString gs, LSystem LS) {
    std::cout<<"innn";
//    int errors = 0;
//
//    GeneticString::node *current_gs_item = new GeneticString::node;
//    current_gs_item = gs.getStart();
//
//    // for each item of the genetic string
//    for (int i = 0; i < gs.count(); i++) {
//
//        // if the item is a component in the alphabet
//        if (LS.getAlphabet().count(current_gs_item->item) > 0) {
//            std::cout << LS.getAlphabet()[current_gs_item->item] << "-yessss" << std::endl;
//
//            DecodedGeneticString::node *component = new DecodedGeneticString::node;
//            // if there are no components in the tree yet
//           // if (this->dgs.getRoot() == NULL) {
//              //  this->dgs->root ==
//
//           // }
//
//        } else {
//            std::cout << "noooo" << std::endl;
//        }
//        current_gs_item = current_gs_item->next;
//    }
}