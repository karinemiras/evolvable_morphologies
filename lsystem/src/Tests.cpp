//
// Created by Karine Miras on 19/05/2017.
//

#include <iostream>
#include <map>

#include "Tests.h"

/**
 * Tests if calculated measures are within the expected range.
 * @id_genome - id of the genome for which the measures were calculated
 * @param measures - list of measures
 * */
void Tests::testMeasures(std::string id_genome, std::map< std::string, double> measures) {

    for( const auto& mea : measures ){ // for each measure

        // if measure is out of the expected range
        if(mea.second < 0 or mea.second > 1) {

            std::cout<< "#TEST ERROR: measure "<<mea.first<<" out of range for genome "<< id_genome <<", with value "<<mea.second;
            exit (EXIT_FAILURE);

        }
    }
}


 