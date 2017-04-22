/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"

using namespace std;


int main(int argc, char* argv[]) {




    std::cout<<" test - step 1";
    LSystem LS;
    LS.build_commands();
    LS.build_alphabet();

    std::cout<<" test - step 2";
    Evolution * e = new Evolution();
    e->readParams();
    std::cout<<" test- step 3";

    e->initPopulation(argc, argv, LS);

    std::cout<<" test- step 4";
    e->measurePopulation( argc, argv);

    std::cout<<" test- step 5";
    e->evaluatePopulation();
    e->selection();

    std::cout<<e->getPopulation().size()<<std::endl;
    e->crossover();
    std::cout<<e->getPopulation().size()<<std::endl;
    e->crossover();
    std::cout<<e->getPopulation().size()<<std::endl;

   // e->testGeneticString(argc, argv,"gecko3", LS);




    return 0;

}


