//
// Created by Karine Miras on 22/06/2017.
//

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Tuning.h"
#include "Evolution.h"


/*
 * Searches for optimum parameters for the novelty search.
 * */
void Tuning::optimize(int argc, char* argv[]){


    std::ofstream history_file;
    std::string path = "../../experiments/history_evolution.txt";
    history_file.open(path);

    std::ofstream evolution_file;
    path = "../../experiments/tuning_evolution.txt";
    evolution_file.open(path);


    int inflimit_num_initial_comp = 1, suplim_num_initial_comp = 10;
    int inflimit_replacement_iterations = 1, suplim_replacement_iterations = 5;

    std::random_device rd;
    std::default_random_engine generator(rd());

    // distributions for random genomes
    std::uniform_real_distribution<double> dist_probs(0, 0.5);
    std::uniform_int_distribution<int> dist_num_initial_comp(inflimit_num_initial_comp, suplim_num_initial_comp);
    std::uniform_int_distribution<int> dist_replacement_iterations(inflimit_replacement_iterations, suplim_replacement_iterations);

    // distributions for mutations
    std::normal_distribution<double> dist_normal(0, 0.3);
    std::uniform_int_distribution<int> dist_increment(-1, 1);


    double max_fitness = 0, average_fitness = 0;

    // initial random population
    for(int i=0; i < this->pop_size; i++){

        // initializes genome with random values
        std::vector<double> genome;
        genome.push_back(dist_num_initial_comp(generator)); // 0 - num_initial_comp
        genome.push_back(dist_replacement_iterations(generator)); // 1 - replacement_iterations
        genome.push_back(dist_probs(generator)); // 2 - mutation_add_prob
        genome.push_back(dist_probs(generator)); // 3 - mutation_delete_prob
        genome.push_back(dist_probs(generator)); // 4 - mutation_move_prob
        genome.push_back(dist_probs(generator)); // 5 - prop_parent
        genome.push_back(dist_probs(generator)); // 6 - prob_add_archive

        // sets up evolution according to the genome (configuration of parameters)
        Evolution * e = new Evolution("config"+std::to_string(i), 1);
        e->updateParameter("num_initial_comp", genome[0]);
        e->updateParameter("replacement_iterations", genome[1]);
        e->updateParameter("mutation_add_prob", genome[2]);
        e->updateParameter("mutation_delete_prob", genome[3]);
        e->updateParameter("mutation_move_prob", genome[4]);
        e->updateParameter("prop_parent", genome[5]);
        e->updateParameter("prob_add_archive", genome[6]);

        // tests fitness of the genome running evolution given the parameters
        int fitness = e->noveltySearch(argc, argv);
        delete e;

        genome.push_back((double)fitness); // 7 - fitness

        this->population.push_back(genome);

        // updates totals
        average_fitness += fitness;
        if(fitness > max_fitness) max_fitness = fitness;

        // saves genome to history of evolution
        history_file<<i<<" "<<genome[0]<<" "<<genome[1]<<" "<<genome[2]<<" "<<genome[3]<<" "<<genome[4]<<" "<<genome[5]<<" "<<genome[6]<<" "<<fitness<<std::endl;
    }

    average_fitness = average_fitness/population.size();
    evolution_file<<"0 "<<max_fitness<<" "<<average_fitness<<std::endl;



    // for each generation
    for(int g=1; g < this->num_generations; g++ ){


        std::vector<std::vector<double>> selected_population;

        // for each expected individual of the offspring
        for(int i=0; i < this->offspring_size; i++){

            int parent1 = this->tournament();
            int parent2 = this->tournament();

            std::cout<<"cross for "<<this->population.size()<<": "<<parent1<<" "<<parent2<<std::endl;

            // crossover: creates new genome with values proportional to the parents
            std::vector<double> genome;
            genome.push_back( round((population[parent1][0]*this->prop_parent) + (population[parent2][0]*(1-this->prop_parent))) ); // 0 - num_initial_comp
            genome.push_back( round((population[parent1][1]*this->prop_parent) + (population[parent2][1]*(1-this->prop_parent))) ); // 1 - replacement_iterations
            genome.push_back( round((population[parent1][2]*this->prop_parent) + (population[parent2][2]*(1-this->prop_parent))) ); // 2 - mutation_add_prob
            genome.push_back( round((population[parent1][3]*this->prop_parent) + (population[parent2][3]*(1-this->prop_parent))) ); // 3 - mutation_delete_prob
            genome.push_back( round((population[parent1][4]*this->prop_parent) + (population[parent2][4]*(1-this->prop_parent))) ); // 4 - mutation_move_prob
            genome.push_back( round((population[parent1][5]*this->prop_parent) + (population[parent2][5]*(1-this->prop_parent))) ); // 5 - prop_parent
            genome.push_back( round((population[parent1][6]*this->prop_parent) + (population[parent2][6]*(1-this->prop_parent))) ); // 6 - prob_add_archive


            // mutation: perturbs values with random values

            if(dist_probs(generator) < this->prob_mu) {

                genome[0] = genome[0] + dist_increment(generator);
                if(genome[0] < inflimit_num_initial_comp) genome[0] = inflimit_num_initial_comp;
                if(genome[0] > suplim_num_initial_comp) genome[0] = suplim_num_initial_comp;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[1] = genome[1] + dist_increment(generator);
                if(genome[1] < inflimit_replacement_iterations) genome[1] = inflimit_replacement_iterations;
                if(genome[1] > suplim_replacement_iterations) genome[1] = suplim_replacement_iterations;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[2] = genome[2] + dist_normal(generator);
                if(genome[2] < 0) genome[2] = 0;
                if(genome[2] > 1) genome[2] = 1;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[3] = genome[3] + dist_normal(generator);
                if(genome[3] < 0) genome[3] = 0;
                if(genome[3] > 1) genome[3] = 1;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[4] = genome[4] + dist_normal(generator);
                if (genome[4] < 0) genome[4] = 0;
                if (genome[4] > 1) genome[4] = 1;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[5] = genome[5] + dist_normal(generator);
                if(genome[5] < 0) genome[5] = 0;
                if(genome[5] > 1) genome[5] = 1;
            }
            if(dist_probs(generator) < this->prob_mu) {

                genome[6] = genome[6] + dist_normal(generator);
                if(genome[6] < 0) genome[6] = 0;
                if(genome[6] > 1) genome[6] = 1;
            }



            Evolution * e = new Evolution("config"+std::to_string(i), 1);
            e->updateParameter("num_initial_comp", genome[0]);
            e->updateParameter("replacement_iterations", genome[1]);
            e->updateParameter("mutation_add_prob", genome[2]);
            e->updateParameter("mutation_delete_prob", genome[3]);
            e->updateParameter("mutation_move_prob", genome[4]);
            e->updateParameter("prop_parent", genome[5]);
            e->updateParameter("prob_add_archive", genome[6]);

            // tests fitness of the genome running evolution given the parameters
            int fitness = e->noveltySearch(argc, argv);
            delete e;

            genome.push_back((double)fitness); // 7 - fitness

            this->population.push_back(genome);

            // saves genome to history of evolution
            history_file<<genome[0]<<" "<<genome[1]<<" "<<genome[2]<<" "<<genome[3]<<" "<<genome[4]<<" "<<genome[5]<<" "<<genome[6]<<" "<<fitness<<std::endl;

        }

        max_fitness = 0, average_fitness = 0;

        // selects population with tournaments
        for(int i=0; i < this->pop_size; i++){

            int genome = tournament();
            selected_population.push_back(this->population[genome]);

            // updates totals
            average_fitness += population[i][7];
            if( population[i][7] > max_fitness) max_fitness =  population[i][7];

        }
        this->population = selected_population;

        average_fitness = average_fitness/population.size();
        evolution_file<<g<<" "<<max_fitness<<" "<<average_fitness<<std::endl;



    }


    history_file.close();
    evolution_file.close();

}


int Tuning::tournament(){

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> dist_tournaments(0, this->pop_size-1);

    // random genomes
    int genome1 = dist_tournaments(generator);
    int genome2 = genome1;
    while(genome2 == genome1) genome2 = dist_tournaments(generator);

    // return genome with highest fitness
    if(this->population[genome1][7] > this->population[genome2][7]){
        return genome1;
    }else{
        return genome2;
    }

}