/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "EvolutionIndirect.h"

#include "mpi.h"



int main(int argc, char* argv[]) {



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
        EvolutionIndirect * e = new EvolutionIndirect("indirect"+std::to_string(my_rank), 1);
        niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);

        std::cout<<">>> experiment "<<std::to_string(my_rank)<<", accumulated niche coverage " << niche_coverage_accumulated<<std::endl;


        MPI_Finalize ();

    // END: parallel repetitions of experiments


//     int niche_coverage_accumulated = 0;
//     EvolutionIndirect * e = new EvolutionIndirect("indirect", 1);
//     niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);
//     std::cout<<">>> accumulated niche coverage " << niche_coverage_accumulated;


    // e->testGeneticString(argc, argv,"spider3");


    return 0;

}


