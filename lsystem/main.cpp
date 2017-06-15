/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"

using namespace std;


int main(int argc, char* argv[]) {


    // experiment: looking for global optimum of innovation
    Evolution * e = new Evolution("mut", 1);
    e->noveltySearch(argc, argv);



    // e->testGeneticString(argc, argv,"spider3");

    return 0;

}


