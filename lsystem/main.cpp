/*
 * Genome indirect enconding: L-System
 * Author: Karine Miras
 * Created: 02/03/17
 */


#include<iostream>

#include "EvolutionIndirect.h"
#include "EvolutionDirect.h"
#include "Tuning.h"


int main(int argc, char* argv[]) {


     //  Tuning t = Tuning();
     //  t.optimize(argc, argv);


    // experiment: looking for global optimum of innovation
     int niche_coverage = 0;
     EvolutionIndirect * e = new EvolutionIndirect("test", 1);
     niche_coverage = e->noveltySearch(argc, argv, 1); // 1-indirect TYPE is temporary, it will be replaced by genome parent class scheme
     std::cout<<">>> niche coverage found "<<niche_coverage;

    // e->testGeneticString(argc, argv,"spider3");


    // experiment: looking for global optimum of innovation
  //   int max_niche_coverage = 0;
  //   EvolutionDirect * e = new EvolutionDirect("test", 1);
   //  max_niche_coverage = e->noveltySearch(argc, argv);
  //   std::cout<<">>> maximum niche coverage found "<<max_niche_coverage;


    return 0;

}


