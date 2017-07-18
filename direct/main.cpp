//
//  main.cpp
//  RobotController
//
//  Created by Berend Weel on 08/04/15.
//  Copyright (c) 2015 Berend Weel. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include "./Genome/RobotRepresentation.h"
#include "./Genome/Mutator.h"
#include "./Genome/EvolverConfiguration.h"

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/shared_ptr.hpp>

#include "./convertion/Evolution.h"
#include "./convertion/Genome.h"
#include "./convertion/DecodedGeneticString.h"
#include "./convertion/LSystem.h";
#include "./convertion/Measures.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace robogen;

int main(int argc,  char* argv[]) {


    // experiment: looking for global optimum of innovation
    int niche_coverage_accumulated = 0;

    Evolution * e = new Evolution("direct", 1);
    // 1-indirect TYPE is temporary, it will be replaced by genome parent class scheme
    niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);

    std::cout<<">>> accumulated niche coverage " << niche_coverage_accumulated;



    return 0;
}