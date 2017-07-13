project(RobotCrossover)
cmake_minimum_required(VERSION 2.6)

#set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
#set(Boost_USE_STATIC_RUNTIME    ON)
#set(Boost_NO_BOOST_CMAKE        ON)
find_package(Boost COMPONENTS system filesystem regex program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})

find_package(OpenSceneGraph REQUIRED)
include_directories(${OPENSCENEGRAPH_INCLUDE_DIRS})

find_package(PkgConfig)

pkg_check_modules(ODE REQUIRED ode)
include_directories(${ODE_INCLUDE_DIRS})

pkg_check_modules(JANSSON REQUIRED jansson)
include_directories(${JANSSON_INCLUDE_DIRS})


set(RobotCrossover_SRCS main.cpp
    Genome/EvolverConfiguration.cpp
    Genome/json2pb.cpp
    Genome/Mutator.cpp
    Genome/NeuralNetwork.c
    Genome/NeuralNetworkRepresentation.cpp
    Genome/NeuronRepresentation.cpp
    Genome/Parameters.cpp
    Genome/PartList.cpp
    Genome/PartRepresentation.cpp
    Genome/RobotRepresentation.cpp
    Genome/robogen.pb.cc
    Genome/ODE_Check/BodyVerifier.cpp
    Genome/ODE_Check/ConfigurationReader.cpp
    Genome/ODE_Check/Model.cpp
    Genome/ODE_Check/Robogen.cpp
    Genome/ODE_Check/RobogenCollision.cpp
    Genome/ODE_Check/RobogenUtils.cpp
    Genome/ODE_Check/Robot.cpp
    Genome/ODE_Check/StartPosition.cpp
    Genome/ODE_Check/models/ActiveCardanModel.cpp
    Genome/ODE_Check/models/ActiveHingeModel.cpp
    Genome/ODE_Check/models/ActiveWheelModel.cpp
    Genome/ODE_Check/models/ActiveWhegModel.cpp
    Genome/ODE_Check/models/BoxObstacle.cpp
    Genome/ODE_Check/models/CardanModel.cpp
    Genome/ODE_Check/models/Connection.cpp
    Genome/ODE_Check/models/CoreComponentModel.cpp
    Genome/ODE_Check/models/HingeModel.cpp
    Genome/ODE_Check/models/ImuSensor.cpp
    Genome/ODE_Check/models/LightSensor.cpp
    Genome/ODE_Check/models/LightSensorModel.cpp
    Genome/ODE_Check/models/LightSource.cpp
    Genome/ODE_Check/models/Motor.cpp
    Genome/ODE_Check/models/ParametricBrickModel.cpp
    Genome/ODE_Check/models/PassiveWheelModel.cpp
    Genome/ODE_Check/models/RotateJointModel.cpp
    Genome/ODE_Check/models/ServoMotor.cpp
    Genome/ODE_Check/models/TouchSensor.cpp
    Genome/ODE_Check/models/TouchSensorModel.cpp
)

include_directories("Transceivers" "Genome" "Genome/ODE_Check")

add_executable(RobotCrossover ${RobotCrossover_SRCS})
target_link_libraries(RobotCrossover ${Boost_LIBRARIES} ${PROTOBUF_LIBRARIES} ${ODE_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES} ${JANSSON_LIBRARIES})
