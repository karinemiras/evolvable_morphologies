//
// Created by Karine Miras on 02/03/2017.
//

#include<iostream>
#include<cstdio>
#include<cstdlib>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;
int main(){

    int element;
    cin>>element;

    string element2;
    ostringstream convert;
    convert << element;
    element2 = convert.str();
    cout << "oi " << element2;

};
