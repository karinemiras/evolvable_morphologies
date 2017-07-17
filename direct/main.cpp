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

#include "./convertion/Genome.h"
#include "./convertion/DecodedGeneticString.h"
#include "./convertion/LSystem.h";
#include "./convertion/Measures.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace robogen;

int main(int argc,  char* argv[]) {

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



    std::string v1 = "../population/guecco.txt";
   // std::string v2 = "../population/guecco.txt";
    std::string v3 = "../Test/evolConf-full.txt";
    std::string v4 = "baby4";


    boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
    boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());
    boost::shared_ptr<RobotRepresentation> robot3(new RobotRepresentation());

    robot1->init(std::string(v1), "P0");
    //robot2->init(std::string(v2), "P1");


    boost::shared_ptr<EvolverConfiguration> evo(new EvolverConfiguration);
    evo->init(std::string(v3));

    boost::random::mt19937 rng(time(0));

    std::string newFileName = "../population/child" + std::string(v4);

   // Mutator mut(evo, rng);
   // mut.createChild(robot1, robot2, newFileName);


    Mutator newr(evo, rng);
    newr.growBodyRandomly(robot1);


    Genome * gen = new Genome("0", "0", "0", 0, 0);
    LSystem LS;
    std::map<std::string, double> params = std::map<std::string, double> ();

    std::string line;
    std::ifstream myfile ("../configuration.txt");

    if (myfile.is_open()) {
        while ( getline (myfile,line) ) {
            std::vector<std::string> tokens;

            boost::split( tokens, line, boost::is_any_of(" ") );

            params[tokens[0]] = std::stod(tokens[1]);
        }
        myfile.close();
    }


    gen->developGenomeIndirect(argc, argv, params, "../population/oi.txt", 1, "direct/offspring");

    Measures * m = new Measures("direct", params);
    m->setGenome(gen);
    // measures phenotype
    m->measurePhenotype(params, "/offspring", 1);



















    return 0;
}