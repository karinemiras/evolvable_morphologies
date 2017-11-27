//
// Created by Karine Miras on 19/05/2017.
//

#include <iostream>
#include <map>

#include "Aux.h"
#include "Tests.h"
#include <math.h>

/**
 * Tests if calculated measures are within the expected range. - for class Measures
 * @id_genome - id of the genome for which the measures were calculated
 * @param measures - list of measures
 * */
void Tests::testMeasures(std::string id_genome, std::map< std::string, double> measures) {

    for( const auto& mea : measures ){ // for each measure

        // if measure is out of the expected range
        if(mea.second < 0 or mea.second > 1 ) {

            std::cout<< "#TEST ERROR: measure "<<mea.first<<" out of range for genome "<< id_genome <<", with value "<<mea.second<<std::endl;
            exit (EXIT_FAILURE);

        }

    }
}


/**
 * Tests if selected parents are different. - for class Evolution
 * @parent1 - id of the genome for parent1
 * @parent21 - id of the genome for parent2
 * */
void Tests::testParents(int parent1, int parent2) {

    if(parent1 == parent2 ) { // if parent are the same genome
        std::cout << " crossover with equal parents " <<parent1<<std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * Tests if population size remains correct.
 **/
void Tests::testPopsize(std::vector<Genome> population, int pop_size){

    if(population.size() > pop_size){
        std::cout << " population size is unappropriate with value " <<population.size()<<std::endl;
        exit(EXIT_FAILURE);
    }
}


void Tests::setParams(std::map<std::string, double> params){

    this->params = params;

}



