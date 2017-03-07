cmake_minimum_required(VERSION 3.6)
project(lsystem_proto)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp LSystem.cpp LSystem.h Genome.cpp Genome.h GeneticString.cpp GeneticString.h)
add_executable(lsystem_proto ${SOURCE_FILES})