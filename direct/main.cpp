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

#include "./convertion/Evolution.h"
#include "./convertion/Genome.h"
#include "./convertion/DecodedGeneticString.h"
#include "./convertion/LSystem.h";
#include "./convertion/Measures.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace robogen;

int main(int argc,  char* argv[]) {

//
//
//    std::string v1 = "../population/guecco.txt";
//   // std::string v2 = "../population/guecco.txt";
//    std::string v3 = "../Test/evolConf-full.txt";
//    std::string v4 = "baby4";
//
//
//    std::ofstream file;
//    file.open(v1);
//    file<<"0 CoreComponent E0 0 GREEN";
//    file.close();
//
//    boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
//    boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());
//    boost::shared_ptr<RobotRepresentation> robot3(new RobotRepresentation());
//
//    robot1->init(std::string(v1), "P0");
//    //robot2->init(std::string(v2), "P1");
//
//
//
//
//    boost::shared_ptr<EvolverConfiguration> evo(new EvolverConfiguration);
//    evo->init(std::string(v3));
//
//    boost::random::mt19937 rng(time(0));
//
//    std::string newFileName = "../population/child" + std::string(v4);
//
//   // Mutator mut(evo, rng);
//   // mut.createChild(robot1, robot2, newFileName);
//
//
//    Mutator newr(evo, rng);
//    newr.growBodyRandomly(robot1);
//
//
//    Genome * gen = new Genome("0", "0", "0", 0, 0);
//    LSystem LS;
//    std::map<std::string, double> params = std::map<std::string, double> ();
//
//    std::string line;
//    std::ifstream myfile ("../configuration.txt");
//
//    if (myfile.is_open()) {
//        while ( getline (myfile,line) ) {
//            std::vector<std::string> tokens;
//
//            boost::split( tokens, line, boost::is_any_of(" ") );
//
//            params[tokens[0]] = std::stod(tokens[1]);
//        }
//        myfile.close();
//    }
//
//
//    gen->developGenomeIndirect(argc, argv, params, "../population/oi.txt", 1, "direct/offspring");
//
//    Measures * m = new Measures("direct", params);
//    m->setGenome(gen);
//    // measures phenotype
//    m->measurePhenotype(params, "/offspring", 1);
//
//
//
//

    // experiment: looking for global optimum of innovation
    int niche_coverage_accumulated = 0;

    Evolution * e = new Evolution("direct", 1);
    // 1-indirect TYPE is temporary, it will be replaced by genome parent class scheme
    niche_coverage_accumulated = e->noveltySearch(argc, argv, 1);

    std::cout<<">>> accumulated niche coverage " << niche_coverage_accumulated;



    return 0;
}