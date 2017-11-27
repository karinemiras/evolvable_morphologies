/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>
#include <random>
#include "EvolutionIndirect.h"


using namespace std;


int main(int argc, char* argv[]) {


//    std::random_device rd;
//    std::default_random_engine generator(rd());
//    std::uniform_int_distribution<int> dist_1(1, 1000000);
//

    EvolutionIndirect e = EvolutionIndirect("locomotion1", 1,
                                                 "locomotion");
    double result = e.runExperiment(argc, argv);
    //std::cout<<result;


//    int niche_coverage_accumulated = 0;
//    EvolutionIndirect *e = new EvolutionIndirect("indirect6", 1, "novelty");
//    niche_coverage_accumulated = e->NS(argc, argv);
//    std::cout << ">>> accumulated niche coverage " << niche_coverage_accumulated;

//    e->testGeneticString(argc, argv,"spider3");


    return 0;

}


