//
// Created by Karine Miras on 15/05/2017.
//


#include <fstream>
#include <iostream>

#include <sys/stat.h>
#include <stdlib.h>

#include "Aux.h"


void Aux::createFolder(std::string dir) {

    std::string pathdir =  "../../experiments/"+dir;
    mkdir(pathdir.c_str(), 0777);

}

void Aux::removeFolder(std::string dir) {

    std::string pathdir =  "../../experiments/"+dir;
    system(("exec rm -r "+pathdir).c_str());

}


void Aux::logs(std::string message){


    if(int(this->params["logs_to_screen"]) == 1) {

        std::cout << message << std::endl;
    }
    if(int(this->params["logs_to_file"]) == 1) {

        std::ofstream logs_file;
        std::string path = "../../experiments/"+ this->experiment_name +"/logs.txt";
        logs_file.open(path, std::ofstream::app);

        logs_file << message << std::endl;

        logs_file.close();
    }

}