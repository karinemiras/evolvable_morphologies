/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"

using namespace std;


int main(int argc, char* argv[]) {


    LSystem LS;
    LS.build_commands();
    LS.build_alphabet();


    Evolution * e = new Evolution();
    e->readParams();

    e->initPopulation(argc, argv, LS);

    e->developPopulation(argc, argv, LS, 1);

//    std::cout<<" -------------------------- crossover "<<std::endl;
//    e->crossover(LS);
//
//    for(int i=0; i<e->getPopulation().size(); i++){
//        std::cout<<"id "<<e->getPopulation()[i]->getId()<<std::endl;
//    }
//
//    e->developPopulation(argc, argv, LS, 2);

    e->measurePopulation(argc, argv, 1);

    e->evaluatePopulation();




   // e->testGeneticString(argc, argv,"gecko3", LS);

    return 0;

}


