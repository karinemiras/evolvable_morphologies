/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"
#include "Tuning.h"


int main(int argc, char* argv[]) {


    Tuning t = Tuning();
    t.optimize(argc, argv);



    // experiment: looking for global optimum of innovation
    // int max_niche_coverage = 0;
    // Evolution * e = new Evolution("test", 1);
    // max_niche_coverage = e->noveltySearch(argc, argv);
    // std::cout<<">>> maximum niche coverage found "<<max_niche_coverage;

    // e->testGeneticString(argc, argv,"spider3");

    return 0;

}


