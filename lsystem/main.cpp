/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 * Edited: 03/03/17
 */



#include<cstdio>
#include<cstdlib>
#include<iostream>
#include <map>
#include <sstream>
#include <vector>



#include "Evolution.h"


using namespace std;


int main(int argc, char* argv[])
{

    std::cout<<" step 1";
    LSystem LS;
    LS.build_commands();
    LS.build_alphabet();
    std::cout<<" step 2";
    Evolution e;
    e.readParams();
    std::cout<<" step 3";
    e.initPopulation(argc, argv, LS);



    return 0;

}


