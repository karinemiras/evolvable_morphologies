/*
 * Genome indirect enconding: L-System
 * Version: V2 - locomotion task in fixed environment (no regulatory rules yet)
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>
#include <random>
#include "EvolutionIndirect.h"


using namespace std;


int main(int argc, char* argv[]) {


    EvolutionIndirect e = EvolutionIndirect("indirect-locomotion", 1);
    e.experiment1(argc, argv);



    return 0;

}


