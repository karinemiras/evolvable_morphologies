project("lsystem_proto")

# CMake useful variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# This sets the include directory for the reference project. This is the -I flag in gcc.
# All the includes should be in this variable To add new directories to the include path,
# just add them in this list (after a space), or add an include_directories statement in a
# subdirectory's CMakeLists.
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/include   # add the binary tree to the search path so that we will find Version.h
)


# MacOS X needed variables
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # adaptable according to individual needs
  set(CMAKE_MACOSX_RPATH ON)
  #    set(CMAKE_SKIP_BUILD_RPATH FALSE)
  #    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  #    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
  #    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif ()

# Finding dependencies
# _____________________________________________________________________________

# Find the boost lib and headers.
set(Boost_NO_BOOST_CMAKE ON) #  Don't do a find_package in config mode before searching for a regular boost install.
find_package(Boost COMPONENTS REQUIRED
        "unit_test_framework"
        )
include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt4 COMPONENTS
        Core
        Widgets
        )


include_directories(SYSTEM ${MPI_INCLUDE_PATH})

find_package(mlpack)

find_package(armadillo)

# Source subdirectories
# _____________________________________________________________________________


include_directories(include)
add_subdirectory(src)
include_directories(../../revolve-simulator)

file(GLOB_RECURSE SOURCES
        main.cpp
        src/GeneticString.cpp
        src/Genome.cpp
        src/LSystem.cpp
        src/Evolution.cpp
        src/EvolutionIndirect.cpp
        src/DecodedGeneticString.cpp
        src/Measures.cpp
        src/Aux.cpp
        src/Tests.cpp
        src/Tuning.cpp
        )

# Generate
# _____________________________________________________________________________

add_executable(lsystem_proto ${SOURCES})

target_link_libraries(lsystem_proto
        Qt4::QtGui
        mlpack
        armadillo
        yaml-cpp
        )
