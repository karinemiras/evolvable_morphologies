/*
 * @(#) Mutator.h   1.0   Sep 2, 2013
 *
 * Titus Cieslewski (dev@titus-c.ch)
 *
 * The ROBOGEN Framework
 * Copyright © 2013-2014 Titus Cieslewski
 *
 * Laboratory of Intelligent Systems, EPFL
 *
 * This file is part of the ROBOGEN Framework.
 *
 * The ROBOGEN Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL)
 * as published by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @(#) $Id$
 */

#ifndef MUTATOR_H_
#define MUTATOR_H_

// enable the following to perform body mutation:
#define BODY_MUTATION //RBP Edit: Seems logical.

#include <boost/random/normal_distribution.hpp>
#include <boost/random/bernoulli_distribution.hpp>

#include "EvolverConfiguration.h"
#include "RobotRepresentation.h"
#include "ODE_Check/BodyVerifier.h"
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#define MAX_MUTATION_ATTEMPTS 100 //TODO move this somewhere else
namespace robogen {

class Mutator {

public:
    
	/**
	 * Creates a Robogen brain mutator with the specified settings
	 * @param pBrainMutate probability for a weight or bias to mutate
	 * @param brainMuteSigma sigma of normal distribution for brain mutation
	 * @param pBrainCrossover probability for crossover among brains
	 */
	Mutator(boost::shared_ptr<EvolverConfiguration> conf,
			boost::random::mt19937 &rng);
    
    Mutator();

	virtual ~Mutator();

	/**
	 * Performs mutation and crossover on a pair of robots
	 */
	boost::shared_ptr<RobotRepresentation> mutate(
			boost::shared_ptr<RobotRepresentation> parent1,
			boost::shared_ptr<RobotRepresentation> parent2);

	void growBodyRandomly(boost::shared_ptr<RobotRepresentation>& robot, std::string v1);
	void randomizeBrain(boost::shared_ptr<RobotRepresentation>& robot);
    
    /**
     * Performs mutation and crossover on a pair of robots, 
     * and writes resulting genome to a file
     */
    void createChild(boost::shared_ptr<RobotRepresentation> robot1,
                     boost::shared_ptr<RobotRepresentation> robot2,
                     std::string filename);

private:

	/**
	 * Mutates a single robot
	 * @return true if robot has been modified
	 * @todo specify bounds in the Neural Network, not here, e.g. with
	 * a MutableDouble class
	 */
	bool mutate(boost::shared_ptr<RobotRepresentation>& robot);

	/**
	 * Performs crossover between two robots
	 * @return true if some crossover has been performed
	 * @todo enable asymmetric crossover
	 */
	bool crossover(boost::shared_ptr<RobotRepresentation>& a,
			boost::shared_ptr<RobotRepresentation>& b);

	/**
	 * Mutation operators
	 */
	bool mutateBrain(boost::shared_ptr<RobotRepresentation>& robot);
	bool mutateBody(boost::shared_ptr<RobotRepresentation>& robot);
	bool removeSubtree(boost::shared_ptr<RobotRepresentation>& robot);
	bool duplicateSubtree(boost::shared_ptr<RobotRepresentation>& robot);
	bool swapSubtrees(boost::shared_ptr<RobotRepresentation>& robot);
    bool crossoverSubtrees(boost::shared_ptr<RobotRepresentation>& robot1,
                           boost::shared_ptr<RobotRepresentation>& robot2);
	bool insertNode(boost::shared_ptr<RobotRepresentation>& robot);
	bool removeNode(boost::shared_ptr<RobotRepresentation>& robot);
	bool mutateParams(boost::shared_ptr<RobotRepresentation>& robot);

	/**
	 * Evolver Configuration
	 */
	boost::shared_ptr<EvolverConfiguration> conf_;

	/**
	 * Random number generator
	 */
	boost::random::mt19937 &rng_;

	/**
	 * Diverse distributions to be used for mutation
	 */
	boost::random::bernoulli_distribution<double> brainMutate_;
	boost::random::normal_distribution<double> normalDistribution_;


	boost::random::bernoulli_distribution<double> weightCrossover_;

	/**
	 * Probability distribution for calling mutation operators
	 */
	boost::random::bernoulli_distribution<double> subtreeRemovalDist_;
	boost::random::bernoulli_distribution<double> subtreeDuplicationDist_;
	boost::random::bernoulli_distribution<double> subtreeSwapDist_;
	boost::random::bernoulli_distribution<double> nodeInsertDist_;
	boost::random::bernoulli_distribution<double> nodeRemovalDist_;
	boost::random::bernoulli_distribution<double> paramMutateDist_;
};

}

#endif /* MUTATOR_H_ */
