//
// Created by Karine Miras on 22/06/2017.
//

#ifndef LSYSTEM_PROTO_TUNING_H
#define LSYSTEM_PROTO_TUNING_H


class Tuning{



    public:

        void optimize(int argc, char* argv[]);
        int tournament();

    private:
        int pop_size = 100;
        int num_generations = 100;
        int offspring_size = 50;
        double prob_mu = 0.3;
        double prop_parent = 0.5;
        std::vector<std::vector<double>> population;

};







#endif LSYSTEM_PROTO_TUNING_H
