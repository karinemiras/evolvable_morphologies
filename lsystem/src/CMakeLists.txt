file(GLOB SOURCES *.cpp)

add_library(lsystem_static STATIC ${SOURCES} DecodedGeneticString.cpp Evolution.cpp Measures.cpp ../test/Dialog.cpp Aux.cpp ../include/Tests.h Tests.cpp)
