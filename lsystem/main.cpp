/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"

using namespace std;


int main(int argc, char* argv[]) {



    Evolution * e = new Evolution();
    e->noveltySearch(argc, argv);



   // e->testGeneticString(argc, argv,"gecko3", LS);

    return 0;

}


