//
// Created by Karine Miras on 15/05/2017.
//



#include <sys/stat.h>

#include <stdlib.h>

#include "Aux.h"

void Aux::createFolder(std::string dir) {

    std::string pathdir =  "../../experiments/"+dir;
    mkdir(pathdir.c_str(), 0777);

}

void Aux::removeFolder(std::string dir) {

    std::string pathdir =  "../../experiments/"+dir;
    system(("exec rm -r "+pathdir+"/*").c_str());

}