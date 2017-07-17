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


//
//    std::string v1 = "../population/child2.txt";
//    std::string v2 = "../population/childbaby.txt";
//    std::string v3 = "../Test/evolConf-full.txt";
//    std::string v4 = "baby2";


//    boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
//    boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());

//    robot1->init(std::string(v1), "P0");
//    robot2->init(std::string(v2), "P1");
//
//    boost::shared_ptr<EvolverConfiguration> evo(new EvolverConfiguration);
//    evo->init(std::string(v3));
//
//    boost::random::mt19937 rng(time(0));
//
//    std::string newFileName = "../population/child" + std::string(v4);
//
//    Mutator mut(evo, rng);
//    mut.createChild(robot1, robot2, newFileName);

//    std::string line;
//    std::ifstream file;
//
//    std::map<std::string, std::string> letters;
//    letters["CoreComponent"] = "C";
//    letters["FixedBrick"] = "B";
//    letters["ActiveHinge"] = "AJ2";
//
//    std::vector<std::string> items;
//
//    file.open("../population/child4.txt");
//    if (!file.is_open()) {
//        std::cout << "Could not open robot text file " << std::endl;
//        return false;
//    }else {
//        std::string mounting_command = "";
//        std::vector<std::string> level_letters;
//
//        int level = 0;
//        int previous_level = -1;
//
//        while (getline(file, line)){
//
//            std::cout <<">>>>>>> "<< line<<std::endl<<std::endl<<std::endl;
//
//            std::vector<std::string> tokens, tokens_level;
//            boost::split( tokens_level, line, boost::is_any_of("\t") );
//            level = tokens_level.size();
//
//            boost::split( tokens, line, boost::is_any_of(" ") );
//
//            int position = std::stoi(tokens[0]);
//            std::string letter = tokens[1];
//            std::string mounting_command = "";
//
//
//            for(int i=0; i<level_letters.size();i++){
//
//                std::cout<<level_letters[i]<<std::endl;
//
//            }
//            std::cout <<std::endl;
//
//            if (level_letters.size()>0) {
//
//std::cout<<"level anterior: "<<level_letters[level_letters.size() - 1] <<std::endl;
//
//                if (level_letters[level_letters.size() - 1] == "CoreComponent") {
//
//                    if (position == 0) mounting_command = "addf";
//                    if (position == 1) mounting_command = "addf";
//                    if (position == 2) mounting_command = "addl";
//                    if (position == 3) mounting_command = "addr";
//                }
//
//                if (level_letters[level_letters.size() - 1] == "FixedBrick") {
//
//                    if (position == 0) mounting_command = "addf";
//                    if (position == 1) mounting_command = "addl";
//                    if (position == 2) mounting_command = "addr";
//                }
//
//                if (level_letters[level_letters.size() - 1] == "ActiveHinge") mounting_command = "addf";
//
//                if(letter!="CoreComponent") items.push_back(mounting_command);
//            }
//
//              std::cout<<"|"<<letter<<"|"<<position<<"|"<<mounting_command<<"|"<<previous_level<<"|"<<level<<"|"<<std::endl;
//
//
//
//            items.push_back(letters[letter]);
//
//
//            if (level > previous_level){
//                level_letters.push_back(letter);
//            }
//            if(level < previous_level){
//
//                int num = (int)  (previous_level - level +1) ;
//
//                for(int j=1; j<=num; j++) {
//                    if (level_letters.size() > 2) {
//                        level_letters.erase(level_letters.begin() + level_letters.size() - 1);
//                    }
//                }
//
//            }
//
//            previous_level = level;
//
//
//
//
//            for(int i=0; i<items.size();i++){
//
//                std::cout<<items[i]<<std::endl;
//
//            }
//
//        }
//
//
//    }
//
//



    Genome gen("0","0","0",0,0);
    LSystem LS;
    std::map<std::string, double> params;

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


    gen.developGenomeIndirect(argc, argv, params, LS, 1, "offspring");





















    return 0;
}