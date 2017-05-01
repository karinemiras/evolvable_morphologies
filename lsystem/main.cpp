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

    e->measurePopulation(argc, argv, 1);

    e->evaluatePopulation();

    e->mutation(LS);

    e->developPopulation(argc, argv, LS, 2);

    e->measurePopulation(argc, argv, 2);

    e->evaluatePopulation();



   // e->testGeneticString(argc, argv,"gecko3", LS);

    return 0;

}


