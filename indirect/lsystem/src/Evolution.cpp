//
// Created by Karine Miras on 21/03/2017.
//

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <sstream>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>


#include <mlpack/core.hpp>
#include <mlpack/methods/neighbor_search/neighbor_search.hpp>

using namespace mlpack;
using namespace mlpack::neighbor; // NeighborSearch and NearestNeighborSort
using namespace mlpack::metric; // EuclideanDistance

#include "Aux.h"
#include "Evolution.h"
#include "Genome.h"
#include "LSystem.h"
#include "Measures.h"


/**
 * Reads parameters from file.
 **/
void Evolution::readParams()
{

  std::string line;
  std::ifstream myfile("../../lsystem/configuration.txt");

  /*   pop_size - size of the population of genomes
  *    offspring_prop - proportion of the population size to dcalculate size of offspring
  *    num_initial_comp - number of initial (random) components in the production rules of the grammar
  *    show_phenotypes - flag to show the phenotype graphic
  *    export_phenotypes - if exports the phenotypes to images (1) or not (0)
  *    export_genomes - if exports the genomes to files (1) or not (0)
  *    replacement_iterations - number of replacement iterations for the l-system
  *    size_component - size of each component in pixels
  *    spacing - spacing between components in pixels
  *    num_generations - number of generations of the evolution
  *    mutation_prob - probability of adding/removing/swaping items (letters/commands) to the genetic-string in the mutation
  *    max_comps - maximum number of components allowed per phenotype
  *    prob_add_archive - probability of adding any genome to the archive
  *    k_neighbors - number of neighbords to compare for fitness
  *    logs_to_screen - if exports the logs to the screen (1) or not (0)
  *    logs_to_file - if exports logs to a file (1) or not (0)
  */


  if (myfile.is_open())
  {
    while (getline(
        myfile,
        line))
    {
      std::vector< std::string > tokens;

      // parameters label and value separated by space
      boost::split(
          tokens,
          line,
          boost::is_any_of(" "));

      // first item is the label, second is the value
      this->params[tokens[0]] = std::stod(tokens[1]);
    }
    myfile.close();
  }
  else
  {
    this->aux.logs("Unable to open parameters file.");
  }

}

/*
 * Change value of parameter for an experiment.
 * */
void Evolution::updateParameter(
    std::string key,
    double value)
{

  this->params[key] = value;
}


/**
 * Loads parameters from saved state.
 **/
void Evolution::loadsParams()
{

  std::string line;
  std::ifstream myfile(
      "../../experiments/"
      + this->experiment_name +
      "/configuration.txt");

  if (myfile.is_open())
  {
    while (getline(
        myfile,
        line))
    {

      std::vector< std::string > tokens;
      // parameters label and value separated by space
      boost::split(
          tokens,
          line,
          boost::is_any_of(" "));

      // first item is the label, second is the value
      this->params[tokens[0]] = std::stod(tokens[1]);
    }
    myfile.close();
  }
  else
  {
    this->aux.logs("Unable to open parameters state.");
  }

}


/**
 * Tries to add individuals to an archive for innovation.
 * @param individuals - list of genomes
 * @param prob_add_archive - probability of adding some genome to the archive of novelty
 * @param path - directory to save backups of the genome of the archive
 **/
void Evolution::addToArchive(
    std::vector< Genome > individuals,
    double prob_add_archive,
    std::string path)
{

  std::random_device rd;

  std::default_random_engine generator(rd());
  // distribution for 0-1 probabilities

  std::uniform_real_distribution< double > prob(
      0.0,
      1.0);

  for (int i = 0;
       i < individuals.size();
       i++)
  {

    // if raffled probability is within the constrained probability
    if (prob(generator) < prob_add_archive)
    {

      //copies object of the genome to archive
      this->archive.emplace(
          individuals[i].getId(),
          individuals[i]);

      if (this->params["export_genomes"] == 1)
      {
        individuals[i].exportGenome(path + "/archive");
      }
    }
  }
}


/* Finds out in which generation the genome was generated.
 * @param idgenome - id of the genome for which to verify generation.
 * */
int Evolution::getGeneration_genome(std::string idgenome)
{

  int generation_genome = 0;
  int offspring_size =
      this->params["pop_size"] * this->params["offspring_prop"];

  // generation of the genome can be found by its id, considering the size of the population and the offspring
  if (this->params["offspring_prop"] == 1)
  {
    generation_genome = (int) trunc(
        std::stoi(idgenome)
        / this->params["pop_size"]) + 1;
  }
  else
  {
    generation_genome = (int) trunc((std::stoi(idgenome) - offspring_size)
                                    / offspring_size) + 1;
  }

  if (generation_genome == 0)
  { generation_genome = 1; }

  return generation_genome;

}


/**
*  Copies the phenotypes of the selection population to a separate folder.
*  @param generation - number of generation in evolution
**/
void Evolution::exportPop(int generation)
{

  std::ofstream measures_file_general;
  std::string path =
      "../../experiments/" + this->experiment_name + "/measures2.txt";
  measures_file_general.open(
      path,
      std::ofstream::app);


  for (int i = 0;
       i < this->population.size();
       i++)
  { // for each genome in the population


    // finds number of generation to which the genome belongs to
    int generation_genome = this->getGeneration_genome(this->population[i].getId());

    std::string filename = "/body_" + this->population[i].getId()
                           + "_p1_" +
                           this->population[i].getId_parent1() +
                           "_p2_" +
                           this->population[i].getId_parent2() +
                           ".png";

    std::string pathfrom = "../../experiments/"
                           + this->experiment_name + "/offspringpop" +
                           std::to_string(generation_genome);

    std::string pathto = "../../experiments/"
                         + this->experiment_name + "/selectedpop" +
                         std::to_string(generation);

    // copies phenotype file from offspring folder to selected population folder
    system(("exec cp " + pathfrom + filename + " " + pathto +
            filename).c_str());

    // copies values of metrics to file of selected population
    std::string line;
    std::ifstream measures(
        "../../experiments/" + this->experiment_name +
        "/offspringpop" +
        std::to_string(generation_genome)
        + "/measures" +
        this->population[i].getId() + ".txt");

    while (getline(
        measures,
        line))
    {

      std::vector< std::string > tokens;
      boost::split(
          tokens,
          line,
          boost::is_any_of(":"));

      measures_file_general << std::to_string(generation) << " "
                            << this->population[i].getId() << " "
                            << tokens[0] << " " << tokens[1] << std::endl;
    }
  }

  measures_file_general.close();

}


/*
 * Compare phenotype of the individual with its parent's.
 * */
double Evolution::compareIndividual(
    Measures m,
    std::string idgenome)
{


  int generation_genome = this->getGeneration_genome(idgenome);

  std::string line;
  std::ifstream measures(
      "../../experiments/" + this->experiment_name +
      "/offspringpop" + std::to_string(generation_genome) +
      "/measures" + idgenome + ".txt");

  double dif = 0;
  while (getline(
      measures,
      line))
  {

    std::vector< std::string > tokens;
    boost::split(
        tokens,
        line,
        boost::is_any_of(":"));

    dif += std::pow(
        m.getGenome()->getMeasures()[tokens[0]] - std::stod(tokens[1]),
        2);
  }
  dif = roundf(std::sqrt(dif) * 100) / 100;

  return dif;

}


/*
 * Compare phenotype of the parents.
 * */
double Evolution::compareParents(
    std::string idparent1,
    std::string idparent2)
{


  int generation_genome_parent1 = this->getGeneration_genome(idparent1);
  std::string line;
  std::ifstream measures(
      "../../experiments/" + this->experiment_name +
      "/offspringpop" +
      std::to_string(generation_genome_parent1) +
      "/measures" + idparent1 + ".txt");

  int generation_genome_parent2 = this->getGeneration_genome(idparent2);
  std::string line2;
  std::ifstream measures2(
      "../../experiments/" + this->experiment_name +
      "/offspringpop" +
      std::to_string(generation_genome_parent2) +
      "/measures" + idparent2 + ".txt");

  double dif = 0;
  while (getline(
      measures,
      line))
  {

    getline(
        measures2,
        line2);

    std::vector< std::string > tokens, tokens2;
    boost::split(
        tokens,
        line,
        boost::is_any_of(":"));
    boost::split(
        tokens2,
        line2,
        boost::is_any_of(":"));

    dif += std::pow(
        std::stod(tokens[1]) - std::stod(tokens2[1]),
        2);
  }
  dif = roundf(std::sqrt(dif) * 100) / 100;

  return dif;

}

/**
 * Measures all the individuals of the population for several metrics.
 *  @param argc - command line parameter
 *  @param argv[] - command line parameter
 *  @param individuals - array with genomes
 *  @param dirpath - name of the output directory
 **/
void Evolution::measureIndividuals(
    int generation,
    std::vector< Genome > &individuals,
    std::string dirpath)
{

  std::ofstream differences_file;
  std::string path =
      "../../experiments/" + this->experiment_name + "/differences.txt";
  differences_file.open(
      path,
      std::ofstream::app);


  // for each genome of the population
  for (int i = 0;
       i < individuals.size();
       i++)
  {

    Measures m = Measures(
        this->experiment_name,
        this->params);
    m.setGenome(individuals[i]);
    // measures phenotype
    m.measurePhenotype(
        this->params,
        dirpath,
        generation);

    // compares measures between individuals
    if (individuals[i].getId_parent1() != "N")
    {

      double dif = this->compareIndividual(
          m,
          individuals[i].getId_parent1());
      differences_file << individuals[i].getId() << " " << dif;

      dif = this->compareIndividual(
          m,
          individuals[i].getId_parent2());
      differences_file << " " << dif;

      dif = this->compareParents(
          individuals[i].getId_parent1(),
          individuals[i].getId_parent2());
      differences_file << " " << dif << std::endl;

    }
  }

  differences_file.close();
}



/*
 *
 * */




/**
 * Creates files of results containing headers.
 */

void Evolution::createHeader()
{

  std::ofstream file;

  std::string path =
      "../../experiments/" + this->experiment_name + "/history.txt";
  file.open(path);
  file
      << "generation idgenome fitgenome idparent1 fitparent1 idparent2 fitparent2 meandif"
      << std::endl;
  file.close();

  path = "../../experiments/" + this->experiment_name + "/evolution.txt";
  file.open(path);
  file
      << "generation maxfitness meanfitness nichecoverage_generation nichecoverage_accumulated";
  for (int i = 0;
       i < this->measures_names.size();
       i++)
  {
    file << " " << this->measures_names[i];
  }
  file << std::endl;
  file.close();

  path = "../../experiments/" + this->experiment_name + "/measures.txt";
  file.open(path);
  file << "generation idgenome";
  for (int i = 0;
       i < this->measures_names.size();
       i++)
  {
    file << " " << this->measures_names[i];
  }
  file << std::endl;
  file.close();

  path = "../../experiments/" + this->experiment_name + "/measures2.txt";
  file.open(path);
  file << "generation genome measures value" << std::endl;
  file.close();

//    path = "../../experiments/"+this->experiment_name+"/nichecoverage_distances.txt";
//    file.open(path);
//    file << "generation average stddev "<<std::endl;
//    file.close();

  path = "../../experiments/" + this->experiment_name + "/differences.txt";
  file.open(path);
  file << "idgenome difference_parent1 difference_parent2 difference_parents"
       << std::endl;
  file.close();


}

void Evolution::saveHistory(
    int generation,
    Genome individual)
{

  std::ofstream history_file;
  std::string path =
      "../../experiments/" + this->experiment_name + "/history.txt";
  history_file.open(
      path,
      std::ofstream::app);

  history_file << std::to_string(generation) << " "     // generation
               << individual.getId() << " "   // idgenome
               << individual.getFitness() << " "  // fitness genome
               << individual.getId_parent1() << " "  // id of parent1
               << individual.getFit_parent1() << " "  // fitness of parent1
               << individual.getId_parent2() << " " // id of parent2
               << individual.getFit_parent2() << " " // fitness of parent2
               << (individual.getFitness() - individual.getFit_parent1())
                  + (individual.getFitness() - individual.getFit_parent2())
                    / (float) 2// mean dif fitness from parents
               << std::endl;

  history_file.close();
}


/**
 * Calculates the fitness for the individuals: average euclidean distance from the nearest neighbors
 * @param generation - number of the generation for the evolution
 * @param individuals_reference - individuals of current-population (parents+offspring)
 * @param individuals_compare - individuals representing current-population+archive
 */

void Evolution::evaluateNS(
    int generation,
    std::vector< Genome > &offspring)
{



  // BEGINNING: auxiliar pointers //

  std::vector< Genome * > individuals_reference = std::vector< Genome * >();
  std::vector< Genome > individuals_compare = std::vector< Genome >();

  for (int j = 0;
       j < this->getPopulation().size();
       j++)
  {

    individuals_reference.push_back(&this->getPopulation()[j]);
    individuals_compare.push_back(this->getPopulation()[j]);

  }

  for (int j = 0;
       j < offspring.size();
       j++)
  {

    individuals_reference.push_back(&offspring[j]);
    individuals_compare.push_back(offspring[j]);
  }

  for (auto &it : this->archive)
  {
    individuals_compare.push_back(it.second);
  }


  // END: auxiliar pointers //

  // matrix with all individuals
  // columns: number of metrics / lines: number of genomes
  arma::mat compare(
      individuals_compare[0].getMeasures().size(),
      individuals_compare.size());

  for (int i = 0;
       i < individuals_compare.size();
       i++)
  {

    int m = 0;
    for (const auto &it : individuals_compare[i].getMeasures())
    {

      compare(
          m,
          i) = it.second;
      m++;
    }

  }


  for (int i = 0;
       i < individuals_reference.size();
       i++)
  {

    // matrix with individuals which will be compared to the others
    // columns: number of metrics / single line: genome
    arma::mat reference(
        individuals_reference[0]->getMeasures().size(),
        1);

    int m = 0;
    for (const auto &it : individuals_reference[i]->getMeasures())
    {

      reference(
          m,
          0) = it.second;
      m++;
    }

    NeighborSearch< NearestNeighborSort, EuclideanDistance > nn(compare);
    arma::Mat< size_t > neighbors;
    arma::mat distances;

    // search for each individual, the nearest neighbors (+1 because it includes itself)
    nn.Search(
        reference,
        this->params["k_neighbors"] + 1,
        neighbors,
        distances);

    double fitness = 0;
    for (size_t j = 0;
         j < neighbors.n_elem;
         ++j)
    {

      fitness += distances[j];
      this->aux.logs(
          "nearest neighbor  " + std::to_string(j) + " for genome " +
          individuals_reference[i]->getId() + " has distance " +
          std::to_string(distances[j]));
    }

    // averages the nearest neighboards
    fitness = fitness / this->params["k_neighbors"];
    individuals_reference[i]->updateFitness(fitness);

    this->saveHistory(
        generation,
        *individuals_reference[i]);


  }


}


/**
 * Evaluates task.
 * @param generation - number of the generation for the evolution
 * @param individuals - population
 * */

void Evolution::evaluateLocomotion(
    int generation,
    std::vector< Genome > &individuals)
{

  for (int i = 0;
       i < individuals.size();
       i++)
  {
    // check validity of the morphology
    if (individuals[i].getValid() == 1)
    {

//      body_spec = get_body_spec(self.conf);
//      brain_spec = get_brain_spec(self.conf);
//      missing_body = yaml_to_robot(body_spec, brain_spec, basic_yaml_object);

      individuals[i].updateFitness(1);
    }
    else
    {
      individuals[i].updateFitness(0);
    }

    this->saveHistory(
        generation,
        individuals[i]);
  }
}


/**
 * Compares average distance among points inthe accumulaetd grid.
 */

void Evolution::compareIndividuals(int generation)
{

  std::ofstream file;
  std::string path = "../../experiments/" + this->experiment_name +
                     "/nichecoverage_distances.txt";
  file.open(
      path,
      std::ofstream::app);


  // fetches points with its dimensions
  std::vector< std::vector< double>> points;
  for (const auto &it : this->morphological_grid_accumulated)
  {

    std::vector< double > dimensions;

    std::vector< std::string > tokens;
    boost::split(
        tokens,
        it.first,
        boost::is_any_of("|"));
    std::vector< std::string > tokens2(
        tokens.begin(),
        tokens.begin() + tokens.size() - 1);

    for (int i = 0;
         i < tokens2.size();
         i++)
    {
      dimensions.push_back(
          std::stod(tokens2[i]) / this->params["grid_bins"]);
    }
    points.push_back(dimensions);
  }


  double avgdistance = 0;
  std::vector< double > avgdistance_points;
  double stddev_distance = 0;

  // for each point
  for (int i = 0;
       i < points.size();
       i++)
  {

    double avgdistance_point = 0;
    // compare to every other point
    for (int j = 0;
         j < points.size();
         j++)
    {

      // for each dimension
      double distance = 0;
      for (int d = 0;
           d < this->population[0].getMeasures().size();
           d++)
      {

        distance += std::pow(
            points[i][d] - points[j][d],
            2);
      }
      // euclidean distance
      distance = std::sqrt(distance);

      avgdistance_point += distance;
    }
    // average distance from the point to all others
    avgdistance_point /= (points.size() - 1);

    avgdistance_points.push_back(avgdistance_point);
    avgdistance += avgdistance_point;
  }

  // average distance of all points
  avgdistance /= points.size();

  for (int i = 0;
       i < avgdistance_points.size();
       i++)
  {
    stddev_distance += std::pow(
        avgdistance - avgdistance_points[i],
        2);
  }
  stddev_distance = std::sqrt(stddev_distance);


  file << generation << " " << avgdistance << " " << stddev_distance
       << std::endl;


  file.close();

}


/**
 * Selects two random genomes and compares their fitness, choosing the winner.
 * @return - the index of the winner genome
 */

int Evolution::tournament()
{

  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution< int > dist_1(
      0,
      (int) this->population.size() -
      1); // size of current pop (parents+offspring)

  int genome1 = dist_1(generator); // random genome 1
  int genome2 = dist_1(generator); // random genome 2

  //random selection test
  //  return dist_1(generator);


  // return the genome with higher fitness / novelty search

  if (this->population[genome1].getFitness() >
      this->population[genome2].getFitness())
  {

    return genome1;
  }
  else
  {
    return genome2;
  }

}


/**
 * Test a genome made from a had-designed genetic-string.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param test_genome - the path with folder/file from where to read the genome to be tested.
 * @param LS - Lsystem structure containing the alphabet.
 */

void
Evolution::testGeneticString(
    int argc,
    char *argv[],
    std::string test_genome)
{

//  this->readParams();
//
//  LSystem LS;
//
//
//  std::string line;
//  std::ifstream myfile("../../experiments/fixed/" + test_genome + ".txt");
//  if (myfile.is_open())
//  {
//    getline(
//        myfile,
//        line);
//    std::vector< std::string > tokens;
//    // items of the genetic-string separated by space
//    boost::split(
//        tokens,
//        line,
//        boost::is_any_of(" "));
//    std::vector< std::string > gs;
//
//    // adds each item in the genetic-string in the array of items
//    for (int j = 0;
//         j < tokens.size();
//         j++)
//    {
//      if (tokens[j] != "")
//      { gs.push_back(tokens[j]); }
//    }
//
//    // creates new genome with id equal 1, using the just read genetic-string
//    Genome gen = Genome(
//        test_genome,
//        test_genome,
//        test_genome,
//        -1,
//        -1);
//
//    gen.setGeneticString(
//        gen.build_genetic_string(
//            gen.getGeneticString(),
//            gs));
//    gen.getGeneticString().display_list();
//
//    // decodes the final genetic-string into a tree of components
//    //std::cout << " >> decoding ... " << std::endl;
//    std::string path = ""; // CHANGE!
//    gen.decodeGeneticString(
//        LS,
//        params,
//        path);
//
//    // generates robot-graphics
//    //std::cout << " >> constructing ... " << std::endl;
//    gen.constructor(
//        argc,
//        argv,
//        this->params,
//        "fixed");
//
//    // measures all metrics od the genome
//    Measures m(
//        this->experiment_name,
//        this->params);
//    m.setGenome(gen);
//    m.measurePhenotype(
//        this->params,
//        "fixed",
//        1);
//
//    myfile.close();
//  }
//  else
//  {
//    this->aux.logs("Cant open genome.");
//  }
}

/**
*  Selection of genomes in a population.
**/

void Evolution::selection()
{
  std::vector< Genome > selected = std::vector< Genome >();
  std::vector< int > index_selected = std::vector< int >();

  // selects genomes, maintaining population size
  for (int i = 0;
       i < this->params["pop_size"];
       i++)
  {
    int genome = this->tournament(); // selects one genome by tournament

    // makes sure that the same genome wont be selected more than once
    while (std::find(
        index_selected.begin(),
        index_selected.end(),
        genome) != index_selected.end())
    {
      genome = this->tournament();
    }
    selected.push_back(this->population[genome]);
    index_selected.push_back(genome);
  }

  this->cleanMemory(index_selected);

  this->population = selected; // substitutes current population for the selected subset

  // # TEST: Tests if population size remains correct.
  this->tests.testPopsize(
      this->population,
      (int) this->params["pop_size"]);
}

/*
 * Deallocate memory used by the non-selected individuals.
 * */

void Evolution::cleanMemory(std::vector< int > index_selected)
{
  // cleaning memory
  std::cout<<"START cleaning memory for non-selected individuals"<<std::endl;
  for (int i = 0;
       i < this->population.size();
       i++)
  {
    // for non-selected individuals
    if(std::find(
        index_selected.begin(),
        index_selected.end(),
        i) == index_selected.end())
    {
      std::cout<<"developed genetic-string"<<std::endl;
      auto item = this->population[i].getGeneticString().getStart();
      while (item not_eq NULL)
      {
        auto item2 = item->next;
        delete item;
        item = item2;
      }

//      std::cout<<"grammar genetic-strings"<<std::endl;
//      for( auto &g: this->population[i].getGrammar())
//      {
//        item = g.second.getStart();
//        while (item not_eq NULL)
//        {
//          auto item2 = item->next;
//          delete item;
//          item = item2;
//        }
//      }

      std::cout<<"decoded genetic-strings"<<std::endl;
      this->cleanVertex(this->population[i].getDgs().getRoot());

      std::cout<<"scene"<<std::endl;
      QList<QGraphicsItem*> all = this->population[i].getScene()->items();
      for (int i = 0; i < all.size(); i++)
      {
        QGraphicsItem *gi = all[i];
        if(gi->parentItem()==NULL) delete gi;
      }
      delete this->population[i].getScene();


    }
  }
  std::cout<<"FINISH cleaning memory for non-selected individuals"<<std::endl;
}

void Evolution::cleanVertex(DecodedGeneticString::Vertex * v){

  if(v != NULL)
  {
    this->cleanVertex(v->left);
    this->cleanVertex(v->front);
    this->cleanVertex(v->right);
    if(v->item == "C")
      this->cleanVertex(v->back);
    delete v;
  }
}

/**
 *  Saves state of the generations to file.
 */

void
Evolution::exportGenerationMetrics(
    int generation,
    std::vector< int > metrics)
{


  std::ofstream evolution_file;
  std::string path =
      "../../experiments/" + this->experiment_name + "/evolution.txt";
  evolution_file.open(
      path,
      std::ofstream::app);

  double maximum_fitness = 0;
  double average_fitness = 0;

  evolution_file << generation << " ";

  for (int i = 0;
       i < this->getPopulation().size();
       i++)
  {


    if (this->getPopulation()[i].getFitness() >
        maximum_fitness)
    {  // finds the maximum fitness of the population

      maximum_fitness = this->getPopulation()[i].getFitness();
    }
    average_fitness += this->getPopulation()[i].getFitness();  //  sums all fitnesses

  }

  average_fitness /= this->getPopulation().size();  // finds the average of the fitnesses

  evolution_file << maximum_fitness << " " << average_fitness;
  for (const auto &m : metrics)
  {
    evolution_file << " " << m;
  }
  evolution_file << std::endl;
  evolution_file.close();

}


void Evolution::setupEvolution()
{

  // read default parameters for the experiment
  this->readParams();

  Aux aux = Aux(
      this->experiment_name,
      this->getParams());
  this->aux = Aux(
      this->experiment_name,
      this->getParams());

  // cleans old files and creates folders for the experiment
  aux.removeFolder(this->experiment_name);

  aux.createFolder(this->experiment_name);
  aux.createFolder(this->experiment_name + "/archive");
  aux.createFolder(this->experiment_name + "/offspringpop1");

  // logs parameters configuration
  this->saveParameters();

}


/**
*  Develops genomes of the population: 1- grows genetic-string of the genome according to grammar, 2- decodes it, 3- constructs the phenotype
*  @param argc - default argument
*  @param argv[] - default argument
*  @param LS - Lsystem structure containing the alphabet
*  @param individuals - array with genomes
**/
void Evolution::developIndividuals(
    int argc,
    char *argv[],
    LSystem LS,
    int generation,
    std::vector< Genome > &individuals,
    std::string path)
{
  // for each genome in the array
  for (size_t i = 0; i < individuals.size(); ++i)
  {
    // develops genome
    individuals[i].developGenomeIndirect(
        argc,
        argv,
        this->params,
        LS,
        generation,
        path);
  }
}

/**
 * Loads population of genomes from files, from previous experiment.
 * @param LS - Lsystem structure containing the alphabet.
 **/
void Evolution::loadPopulation(int generation)
{

  // deletes possible remains of unfinished generation
  std::string pathdir =
      "../../experiments/" + this->experiment_name + "/selectedpop" +
      std::to_string(generation + 1);
  system(("exec rm -r " + pathdir).c_str());
  pathdir = "../../experiments/" + this->experiment_name + "/offspringpop" +
            std::to_string(generation + 1);
  system(("exec rm -r " + pathdir).c_str());

  // generates list of files (genomes of last population)
  std::system(("ls ../../experiments/" + this->experiment_name +
               "/selectedpop" + std::to_string(generation) +
               ">../../experiments/" + this->experiment_name +
               "/temp.txt").c_str());

  std::ifstream listgenomes(("../../experiments/" + this->experiment_name +
                             "/temp.txt").c_str());
  std::string linegenome;

  //for each file (genome)
  while (getline(
      listgenomes,
      linegenome))
  {
    std::vector< std::string > tokens;
    boost::split(
        tokens,
        linegenome,
        boost::is_any_of("_."));

    std::string idgenome = tokens[1];
    std::string idparent1 = tokens[3];
    std::string idparent2 = tokens[5];

    // recreates genome back to population
    // fitness of the parents is not loaded, but it doesnt matter, because at this point it has been saved to the history already
    Genome gen = Genome(
        idgenome,
        idparent1,
        idparent2,
        -1,
        -1);

    // finds number of generation to which the genome belongs to
    int generation_genome = 0;
    int offspring_size =
        this->params["pop_size"] * this->params["offspring_prop"];
    if (this->params["offspring_prop"] == 1)
    {
      generation_genome = (int) trunc(
          std::stoi(idgenome)
          / this->params["pop_size"]) + 1;
    }
    else
    {
      generation_genome =
          (int) trunc((std::stoi(idgenome) - offspring_size)
                      / offspring_size) + 1;
    }

    if (generation_genome == 0)
    { generation_genome = 1; }

    std::ifstream listalphabet(
        "../../experiments/" + this->experiment_name + "/offspringpop" +
        std::to_string(generation_genome) + "/genome" + idgenome +
        ".txt");
    std::string linealphabet;
    // for each letter of the alphabet
    while (getline(
        listalphabet,
        linealphabet))
    {

      // gets letter and production rile from file
      std::vector< std::string > items;
      boost::split(
          items,
          linealphabet,
          boost::is_any_of(" "));
      std::vector< std::string > items_rule(
          items.begin() + 1,
          items.begin() + items.size() -
          1);

      // build a genetic-string with the production rule for the letter
      auto lgs =  GeneticString();
      lgs = gen.build_genetic_string(
          lgs,
          items_rule);

      // adds letter and its production rule (made a genetic-string) to the grammar of the genome
      gen.addLetterGrammar(
          items[0],
          lgs);

    }

    std::ifstream listmeasures(
        "../../experiments/" + this->experiment_name + "/offspringpop" +
        std::to_string(generation_genome) + "/measures" + idgenome +
        ".txt");
    std::string linemeasures;
    // for each measure of the list
    while (getline(
        listmeasures,
        linemeasures))
    {

      std::vector< std::string > tokens;
      boost::split(
          tokens,
          linemeasures,
          boost::is_any_of(":"));

      gen.updateMeasure(
          tokens[0],
          std::stod(tokens[1]));
    }

    this->population.push_back(gen);  // adds genome to the population

  }


};

/**
 * Loads archive of genomes from files, from previous experiment.
 * @param LS - Lsystem structure containing the alphabet.
 **/

void Evolution::loadArchive()
{

  // generates list of files (genomes of last archive)
  std::system(("ls ../../experiments/" + this->experiment_name + "/archive" +
               ">../../experiments/" + this->experiment_name +
               "/temp.txt").c_str());

  std::ifstream listgenomes(("../../experiments/" + this->experiment_name +
                             "/temp.txt").c_str());
  std::string linegenome;

  //for each file (genome)
  while (getline(
      listgenomes,
      linegenome))
  {

    linegenome = linegenome.substr(
        6,
        linegenome.size());

    std::vector< std::string > tokens;
    boost::split(
        tokens,
        linegenome,
        boost::is_any_of("."));

    std::string idgenome = tokens[0];

    // recreates genome back to population
    // fitness of the parents is not loaded, but it doesnt matter, because at this point it has been saved to the history already
    Genome gen = Genome(
        idgenome,
        "N",
        "N",
        -1,
        -1);


    // finds number of generation to which the genome belongs to
    int generation_genome = 0;
    int offspring_size =
        this->params["pop_size"] * this->params["offspring_prop"];
    if (this->params["offspring_prop"] == 1)
    {
      generation_genome = (int) trunc(
          std::stoi(idgenome) / this->params["pop_size"]) + 1;
    }
    else
    {
      generation_genome = (int) trunc(
          (std::stoi(idgenome) - offspring_size) / offspring_size) +
                          1;
    }


    std::ifstream listalphabet(
        "../../experiments/" + this->experiment_name +
        "/archive/genome" + idgenome + ".txt");
    std::string linealphabet;
    // for each letter of the alphabet
    while (getline(
        listalphabet,
        linealphabet))
    {

      // gets letter and production rile from file
      std::vector< std::string > items;
      boost::split(
          items,
          linealphabet,
          boost::is_any_of(" "));
      std::vector< std::string > items_rule(
          items.begin() + 1,
          items.begin() + items.size() -
          1);

      // build a genetic-string with the production rule for the letter
      GeneticString   lgs;
      lgs = gen.build_genetic_string(
          lgs,
          items_rule);

      // adds letter and its production rule (made a genetic-string) to the grammar of the genome
      gen.addLetterGrammar(
          items[0],
          lgs);

    }

    std::ifstream listmeasures(
        "../../experiments/" + this->experiment_name + "/offspringpop" +
        std::to_string(generation_genome) + "/measures" + idgenome +
        ".txt");
    std::string linemeasures;
    // for each measure of the list
    while (getline(
        listmeasures,
        linemeasures))
    {

      std::vector< std::string > tokens;
      boost::split(
          tokens,
          linemeasures,
          boost::is_any_of(":"));

      gen.updateMeasure(
          tokens[0],
          std::stod(tokens[1]));
    }

    this->archive.emplace(
        idgenome,
        gen);  // adds genome to the archive

  }


};


/**
 *  Loads state of previous experiment.
 **/
int Evolution::loadExperiment()
{

  this->logsTime("start");

  // loads state of parameters from previous experiment
  this->loadsParams();
  this->aux = Aux(
      this->experiment_name,
      this->getParams());


  // loads generation number from previous  experiment
  int gi = std::stoi(this->readsEvolutionState()[0]);
  // loads next_id from previous experiment
  this->next_id = std::stoi(this->readsEvolutionState()[1]);

  // loads state of the morphological_grid_accumulated
  std::string line;
  std::ifstream myfile(
      "../../experiments/" + this->experiment_name +
      "/morphological_grid_accumulated.txt");
  while (getline(
      myfile,
      line))
  {
    std::vector< std::string > tokens, tokens2;
    boost::split(
        tokens,
        line,
        boost::is_any_of("-"));
    boost::split(
        tokens2,
        tokens[1],
        boost::is_any_of(" "));
    std::vector< std::string > tokens3(
        tokens2.begin(),
        tokens2.begin() + tokens2.size() -
        1);
    std::vector< std::string > points;
    for (int i = 0;
         i < tokens3.size();
         i++)
    {
      points.push_back(tokens3[i]);
    }
    this->morphological_grid_accumulated[tokens[0]] = points;
  }
  myfile.close();


  // loads experiment  population
  this->loadPopulation(gi);

  if (this->type_experiment == "novelty")
  {
    // loads experiment  archive
    this->loadArchive();
  }

  return gi;

}

/**
 *  Starts new experiment.
 **/
int Evolution::initExperiment(
    int argc,
    char *argv[],
    LSystem LS)
{

  // reads parameters for new experiment and creates directories
  this->setupEvolution();

  this->logsTime("start");
  this->createHeader();

  int gi = 1; // start evolution from first generation

  this->aux.logs(
      "---------------- generation " + std::to_string(gi) +
      " ----------------");

  // initializes population
  this->initPopulation(LS);

  // develops genomes of the initial population
  this->developIndividuals(
      argc,
      argv,
      LS,
      gi,
      this->population,
      this->experiment_name + "/offspringpop");

  // measures phenotypes of the individuals
  this->measureIndividuals(
      gi,
      this->population,
      "/offspringpop");

  if (this->type_experiment == "novelty")
  {
    // updates the average measures for the population
    this->evaluateNS(
        gi,
        this->population);

    // (possibly) adds genome to archive
    this->addToArchive(
        this->population,
        this->params["prob_add_archive"],
        this->experiment_name);
  }

  if (this->type_experiment == "locomotion")
  {
    // updates the average measures for the population
    this->evaluateLocomotion(
        gi,
        this->population);
  }

  return gi;
}


/**
*  Evolution in the search for novelty.
**/

double Evolution::runExperiment(
    int argc,
    char *argv[])
{

  // loads alphabet with letters and commands
  LSystem LS;


  int gi = 0; // initial generation

  // if experiment is set to start from the beginning
  if (this->new_experiment == 1)
  {
    gi = this->initExperiment(
        argc,
        argv,
        LS);

    // saves metrics of evolution to file
    this->exportGenerationMetrics(
        gi,
        this->calculateNicheCoverage(this->population));

    // if experiment is set to continue from previous experiment
  }
  else
  {
    gi = this->loadExperiment();
  }

  // evolves population through out generations
  for (int g = gi + 1; g <= params["num_generations"]; ++g)
  {
    this->aux.logs(
        "---------------- generation " + std::to_string(g) +
        " ----------------");

    std::vector< Genome > offspring = std::vector< Genome >();

    // creates offspring
    this->crossover(
        LS,
        offspring);
    this->aux.createFolder(
        this->experiment_name + "/offspringpop" + std::to_string(g));
    this->aux.createFolder(
        this->experiment_name + "/selectedpop" + std::to_string(g));

    // develops genomes of the offspring
    this->developIndividuals(
        argc,
        argv,
        LS,
        g,
        offspring,
        this->experiment_name + "/offspringpop");

    // measures phenotypes of the offspring
    this->measureIndividuals(
        g,
        offspring,
        "/offspringpop");


    if (this->type_experiment == "locomotion")
    {
      // evaluates offspring
      this->evaluateLocomotion(
          g,
          offspring);
    }
    if (this->type_experiment == "novelty")
    {
      // evaluates population (parents+offspring)
      this->evaluateNS(
          g,
          offspring);
    }

    // adds new individuals to population
    for (int j = 0;
         j < offspring.size();
         j++)
    {
      this->population.push_back(offspring[j]);
    }

    std::vector< int > niche_measures = this->calculateNicheCoverage(offspring);

    // selects individuals, keeping the population with a fixed size
    this->selection();

    // saves metrics of evolution to file
    this->exportGenerationMetrics(
        g,
        niche_measures);

    // saves phenotypes of the selected population to a separated folder (only for visualization issues)
    this->exportPop(g);


    // saves the number of the last generation created/evaluated
    this->writesEvolutionState(
        g,
        this->next_id);


  }

  this->summaryNicheCoverage();


  this->logsTime("end");

  return 0.0; // FIX!!


}


void Evolution::summaryNicheCoverage()
{

  std::ofstream file;

  std::string path = "../../experiments/" + this->experiment_name +
                     "/morphological_grid_summary.txt";
  file.open(path);
  file << "point count" << std::endl;

  for (const auto &it : this->morphological_grid_accumulated)
  {

    file << it.first + " " << it.second.size() << std::endl;
  }
  file.close();

}


std::vector< Genome > Evolution::getPopulation()
{
  return this->population;
}

std::map< std::string, double > Evolution::getParams()
{
  return params;
}


/*
 * Exports the parameters of the experiment.
 **/
void Evolution::saveParameters()
{

  std::ofstream param_file;
  std::string path =
      "../../experiments/" + this->experiment_name + "/configuration.txt";
  param_file.open(path);

  // writes each parameter to a different line in a the file
  for (auto &it : this->getParams())
  {

    param_file << it.first << " " << it.second;
    param_file << std::endl;
  }
  param_file.close();
}


/*
 * Logs time.
 **/
void Evolution::logsTime(std::string moment)
{

  time_t sta = time(0);
  char *dtsta = ctime(&sta);
  this->aux.logs("experiment " + moment + ": " + dtsta);

}

/*
 * Logs the generation from which the recovered evolution should start from.
 * */
void Evolution::writesEvolutionState(
    int generation,
    int next_id)
{

  std::ofstream logs_file;
  std::string path = "../../experiments/" + this->experiment_name +
                     "/evolutionstate.txt";
  logs_file.open(path);
  logs_file << generation << " " << next_id;
  logs_file.close();
}

/*
 * Reads number of the generation from which the recovered evolution should start from.
 * */
std::vector< std::string > Evolution::readsEvolutionState()
{

  std::string line;
  std::ifstream myfile(
      "../../experiments/" + this->experiment_name +
      "/evolutionstate.txt");
  if (myfile.is_open())
  {

    getline(
        myfile,
        line);
    std::vector< std::string > tokens;
    // parameters label and value separated by space
    boost::split(
        tokens,
        line,
        boost::is_any_of(" "));

    return tokens;

  }
  else
  {
    this->aux.logs("Unable to open evolutionstate file.");
  }
  myfile.close();

}


/*
 * Calculates the quality metric for the novelty search: niche coverage and bins of measures
 * */
std::vector< int >
Evolution::calculateNicheCoverage(std::vector< Genome > individuals)
{
  for (int i = 0;
       i < individuals.size();
       i++)
  {

    std::string key_point = "";
    double distance = 0;

    // for each measure (dimension)
    for (const auto &it : individuals[i].getMeasures())
    {
      // accounts for NC
      // for each bin
      for (int b = 1;
           b <= this->params["grid_bins"];
           b++)
      {
        // if value is zero, sets into the first bin
        if (it.second == 0 and b == 1)
        {

          key_point += std::to_string(b) + "|";
          distance +=
              -1 * (it.second - (b / this->params["grid_bins"]));
        }
        // otherwise, sets value for measure into the correct bin
        if (it.second > ((b - 1) / this->params["grid_bins"]) and
            it.second <= (b / this->params["grid_bins"]))
        {
          key_point += std::to_string(b) + "|";
          distance +=
              -1 * (it.second - (b / this->params["grid_bins"]));
        }
      }

      // accounts for occurrence of value for measure
      if (std::find(
          this->morphological_measures_accumulated[it.first].begin(),
          this->morphological_measures_accumulated[it.first].end(),
          it.second)
          == this->morphological_measures_accumulated[it.first].end())
      {

        this->morphological_measures_accumulated[it.first].push_back(it.second);
      }
    }

    // if point already exists in the array, adds an individual and the id
    if (this->morphological_grid_accumulated.count(key_point) > 0)
    {

      this->morphological_grid_accumulated[key_point].push_back(individuals[i].getId());

      // if point does not exist in the array yet, , adds new point with its first individual and the difference between them
    }
    else
    {
      std::vector< std::string > individual;
      individual.push_back(individuals[i].getId());
      this->morphological_grid_accumulated[key_point] = individual;
    }
  }



  // logs state of the grid
  std::ofstream myfile;
  std::string path = "../../experiments/" + this->experiment_name +
                     "/morphological_grid_accumulated.txt";
  myfile.open(path);
  for (const auto &it : this->morphological_grid_accumulated)
  {
    myfile << it.first + "-";
    for (int i = 0;
         i < it.second.size();
         i++)
    {
      myfile << it.second[i] << " ";
    }
    myfile << std::endl;
  }
  myfile.close();


  std::vector< int > morphological_grids;
  morphological_grids.push_back((int) this->morphological_grid_generation.size());  // pos 0
  morphological_grids.push_back((int) this->morphological_grid_accumulated.size()); // pos 1
  for (const auto &it :this->morphological_measures_accumulated)
  {
    morphological_grids.push_back((int) it.second.size());
  }

  return morphological_grids;
}

