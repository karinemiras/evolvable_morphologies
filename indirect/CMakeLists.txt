cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -pedantic -Wall")

# enable_testing() needed on top-level CMakeLists.txt
enable_testing()

# Create target for generating documentation
# add_custom_target(doc)
# add_dependencies(doc docLSystem)

add_subdirectory(lsystem)
