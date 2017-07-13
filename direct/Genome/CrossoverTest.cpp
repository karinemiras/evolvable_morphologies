#include <iostream>
#include <string>
#include "RobotRepresentation.h"
#include <boost/shared_ptr.hpp>

using namespace robogen;

int main(int argc, const char *argv[])
{
	boost::shared_ptr<RobotRepresentation> robot1(new RobotRepresentation());
	boost::shared_ptr<RobotRepresentation> robot2(new RobotRepresentation());
	
    robot1->init(argv[1]);
    robot2->init(argv[2]);
    
    return 0;
}

/* g++ cpptest.cpp RobotRepresentation.cpp PartRepresentation.cpp NeuralNetworkRepresentation.cpp NeuronRepresentation.cpp robogen.pb.cc PartList.cpp EvolverConfiguration.cpp Mutator.cpp BodyVerifier.cpp -o cpptest -L/usr/local/lib -lboost_program_options -lprotobuf -lboost_filesystem -lboost_regex -lboost_system */