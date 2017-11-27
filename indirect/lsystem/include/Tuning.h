//
// Created by Karine Miras on 22/06/2017.
//

#ifndef LSYSTEM_PROTO_TUNING_H
#define LSYSTEM_PROTO_TUNING_H

#include <vector>

#include "EvolutionIndirect.h"

class Tuning
{
public:

  void optimize(
      int argc,
      char *argv[]);

  int tournament();

  void setupEvolution(
      EvolutionIndirect *e,
      std::vector< double > genome);

private:
  size_t pop_size = 30;
  size_t num_generations = 100;
  size_t offspring_size = 15;
  double prob_mu = 0.3;
  double prop_parent = 0.5;
  std::vector< std::vector< double>> population;

};

#endif LSYSTEM_PROTO_TUNING_H
