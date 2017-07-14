/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "EvolutionIndirect.h"
#include "EvolutionDirect.h"
#include "Tuning.h"


int main(int argc, char* argv[]) {


     // experiment: looking for global optimum of innovation
     int niche_coverage_accumulated = 0;

     EvolutionIndirect * e = new EvolutionIndirect("test_max", 1);
     // 1-indirect TYPE is temporary, it will be replaced by genome parent class scheme
     niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);

     std::cout<<">>> accumulated niche coverage " << niche_coverage_accumulated;


    // e->testGeneticString(argc, argv,"spider3");


    return 0;

}


