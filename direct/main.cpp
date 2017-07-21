//
//  main.cpp
//  RobotController
//
//  Created by Berend Weel on 08/04/15.
//  Copyright (c) 2015 Berend Weel. All rights reserved.
//

#include "mpi.h"

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

    // BEGIN: parallel repetitions of experiments

        int rc, num_tasks , my_rank;
        rc = MPI_Init( &argc, &argv);

        if (rc != MPI_SUCCESS) {
            fprintf(stderr, "Unable to set up MPI");
            MPI_Abort(MPI_COMM_WORLD, rc);
        }

        MPI_Comm_size( MPI_COMM_WORLD , &num_tasks);
        MPI_Comm_rank( MPI_COMM_WORLD , &my_rank);


        // runs experiment
        int niche_coverage_accumulated = 0;
        Evolution * e = new Evolution("direct"+std::to_string(my_rank), 1);
        niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);

        std::cout<<">>> experiment "<<std::to_string(my_rank)<<", accumulated niche coverage " << niche_coverage_accumulated<<std::endl;


        MPI_Finalize ();

    // END: parallel repetitions of experiments



//    int niche_coverage_accumulated = 0;
//    Evolution * e = new Evolution("direct_Test", 1);
//    niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);
//    std::cout<<">>> experiment, accumulated niche coverage " << niche_coverage_accumulated<<std::endl;

    return 0;
}