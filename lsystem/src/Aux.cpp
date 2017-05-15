//
// Created by Karine Miras on 15/05/2017.
//



#include <sys/stat.h>

#include "Aux.h"

void Aux::createFolder(std::string dir) {

    std::string pathdir =  "../../tests/"+dir;
    mkdir(pathdir.c_str(), 0777);

}