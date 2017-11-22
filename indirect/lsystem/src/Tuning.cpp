//
// Created by Karine Miras on 22/06/2017.
//

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "Tuning.h"
#include "EvolutionIndirect.h"


/* UNFINISHED TEST!
 * Searches for optimum parameters for the novelty search.
 * WARNING: comment line this->readParams(); in void Evolution::setupEvolution(), for this method to work.
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
    std::uniform_real_distribution<double> dist_probs(0,1);

    // distributions for random genomes
    std::uniform_int_distribution<int> dist_mut(1, 4);
    std::uniform_int_distribution<int> dist_num_initial_comp(inflimit_num_initial_comp, suplim_num_initial_comp);
    std::uniform_int_distribution<int> dist_replacement_iterations(inflimit_replacement_iterations, suplim_replacement_iterations);

    // distributions for mutations
    std::normal_distribution<double> dist_normal(0, 0.2);
    std::uniform_int_distribution<int> dist_increment(-1, 1);


    double max_fitness = 0, average_fitness = 0;

    // initial random population
    for(int i=0; i < this->pop_size; i++){

        // initializes genome with random values
        std::vector<double> genome;
        genome.push_back(dist_num_initial_comp(generator)); // 0 - num_initial_comp
        genome.push_back(dist_replacement_iterations(generator)); // 1 - replacement_iterations
        genome.push_back(dist_mut(generator)/(float)10); // 2 - mutation_add_prob
        genome.push_back(dist_mut(generator)/(float)10); // 3 - mutation_delete_prob
        genome.push_back(dist_mut(generator)/(float)10); // 4 - mutation_move_prob

        // sets up evolution according to the genome (configuration of parameters)
        EvolutionIndirect * e = new EvolutionIndirect
                ("g0-config"+std::to_string(i), 1,"novelty");
        this->setupEvolution(e, genome);

        // tests fitness of the genome running evolution given the parameters
        double fitness = e->runExperiment(argc, argv);
        delete e;

        genome.push_back((double)fitness); // 5 - fitness

        this->population.push_back(genome);

        // updates totals
        average_fitness += fitness;
        if(fitness > max_fitness) max_fitness = fitness;

        // saves genome to history of evolution
        history_file<<genome[0]<<" "<<genome[1]<<" "<<genome[2]<<" "<<genome[3]<<" "<<genome[4]<<" "<<genome[5]<<std::endl;
    }

    average_fitness = average_fitness/population.size();
    evolution_file<<"0 "<<max_fitness<<" "<<average_fitness<<std::endl;



    // for each generation
    for(int g=1; g < this->num_generations; g++ ){


        // for each expected individual of the offspring
        for(int i=0; i < this->offspring_size; i++){

            int parent1 = this->tournament();
            int parent2 = this->tournament();

            //std::cout<<"cross for "<<this->population.size()<<": "<<parent1<<" "<<parent2<<std::endl;

            // crossover: creates new genome with values proportional to the parents
            std::vector<double> genome;
            genome.push_back( round((population[parent1][0]*this->prop_parent) + (population[parent2][0]*(1-this->prop_parent))) ); // 0 - num_initial_comp
            genome.push_back( round((population[parent1][1]*this->prop_parent) + (population[parent2][1]*(1-this->prop_parent))) ); // 1 - replacement_iterations
            genome.push_back( (population[parent1][2]*this->prop_parent) + (population[parent2][2]*(1-this->prop_parent)) ); // 2 - mutation_add_prob
            genome.push_back( (population[parent1][3]*this->prop_parent) + (population[parent2][3]*(1-this->prop_parent)) ); // 3 - mutation_delete_prob
            genome.push_back( (population[parent1][4]*this->prop_parent) + (population[parent2][4]*(1-this->prop_parent)) ); // 4 - mutation_move_prob


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


            EvolutionIndirect * e = new EvolutionIndirect("g"+std::to_string
                    (g)+"-config"+std::to_string(i), 1, "novelty");
            this->setupEvolution(e, genome);

            // tests fitness of the genome running evolution given the parameters
            double fitness = e->runExperiment(argc, argv);
            delete e;


            genome.push_back((double)fitness); // 5 - fitness

            this->population.push_back(genome);

            // saves genome to history of evolution
            history_file<<genome[0]<<" "<<genome[1]<<" "<<genome[2]<<" "<<genome[3]<<" "<<genome[4]<<" "<<genome[5]<<std::endl;

        }

        max_fitness = 0, average_fitness = 0;

        std::vector<std::vector<double>> selected_population;

        // selects population with tournaments
        for(int i=0; i < this->pop_size; i++){

            int genome = tournament();
            //while(std::count(selected_population.begin(), selected_population.end(), genome) > 1) genome = tournament();

            selected_population.push_back(this->population[genome]);

            // updates totals
            average_fitness += population[i][5];
            if( population[i][5] > max_fitness) max_fitness =  population[i][5];

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
    std::uniform_int_distribution<int> dist_tournaments(0, (int)this->population.size()-1);

    // random genomes
    int genome1 = dist_tournaments(generator);
    int genome2 = dist_tournaments(generator);
    while(genome2 == genome1) genome2 = dist_tournaments(generator);

    // return genome with highest fitness
    if(this->population[genome1][5] > this->population[genome2][5]){
        return genome1;
    }else{
        return genome2;
    }

}


void Tuning::setupEvolution(EvolutionIndirect * e, std::vector<double> genome){

    e->updateParameter("num_initial_comp", genome[0]);
    e->updateParameter("replacement_iterations", genome[1]);
    e->updateParameter("mutation_add_prob", genome[2]);
    e->updateParameter("mutation_delete_prob", genome[3]);
    e->updateParameter("mutation_move_prob", genome[4]);

    e->updateParameter("pop_size", 30);
    e->updateParameter("num_generations", 10);
    e->updateParameter("prop_parent", 0.5);
    e->updateParameter("prob_add_archive", 0.1);
    e->updateParameter("offspring_size", 0.5);
    e->updateParameter("k_neighbors", 15);
    e->updateParameter("max_comps", 30);
    e->updateParameter("grid_bins", 100);
    e->updateParameter("logs_to_screen", 0);
    e->updateParameter("logs_to_file", 1);
    e->updateParameter("size_component", 40);
    e->updateParameter("spacing", 1);
    e->updateParameter("show_phenotypes", 0);
    e->updateParameter("export_phenotypes", 1);
    e->updateParameter("export_genomes", 1);

}