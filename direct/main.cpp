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

using namespace robogen;

int main(int argc, const char * argv[]) {

//    if (argc < 5) {
//        std::cout << "Arguments missing. Proper usage is: ./main [robot file name] [robot file name] [conf file name] [child suffix]" << std::endl;
//
//        return 1;
//    }
//
//    boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
//    boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());
//
//    robot1->init(std::string(argv[1]), "P0");
//    robot2->init(std::string(argv[2]), "P1");
//
//    boost::shared_ptr<EvolverConfiguration> evo(new EvolverConfiguration);
//    evo->init(std::string(argv[3]));
//
//    boost::random::mt19937 rng(time(0));
//
//    std::string newFileName = "child" + std::string(argv[4]);
//
//    Mutator mut(evo, rng);
//    mut.createChild(robot1, robot2, newFileName);
//



    std::string v1 = "../population/child2.txt";
    std::string v2 = "../population/childbaby.txt";
    std::string v3 = "../Test/evolConf-full.txt";
    std::string v4 = "baby2";


    boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
    boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());

    robot1->init(std::string(v1), "P0");
    robot2->init(std::string(v2), "P1");

    boost::shared_ptr<EvolverConfiguration> evo(new EvolverConfiguration);
    evo->init(std::string(v3));

    boost::random::mt19937 rng(time(0));

    std::string newFileName = "../population/child" + std::string(v4);

    Mutator mut(evo, rng);
    mut.createChild(robot1, robot2, newFileName);

    return 0;
}