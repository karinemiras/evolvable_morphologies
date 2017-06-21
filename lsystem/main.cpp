/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "Evolution.h"


#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // ManhattanDistance


int main(int argc, char* argv[]) {


    // experiment: looking for global optimum of innovation
     Evolution * e = new Evolution("test", 1);
     e->noveltySearch(argc, argv);

    // e->testGeneticString(argc, argv,"spider3");

    return 0;

}


