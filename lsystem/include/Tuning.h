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
        int pop_size = 5;
        int num_generations = 2;
        int offspring_size = 1;
        double prob_mu = 1;
        double prop_parent = 0.5;
        std::vector<std::vector<double>> population;

};







#endif LSYSTEM_PROTO_TUNING_H
