//
// Created by Karine Miras on 12/04/2017.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <map>
#include <math.h>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "Genome.h"
#include "Measures.h"
#include "LSystem.h"






void Measures::setGenome(Genome * gen){
    this->gen = gen;
}




/**
 * Calculates several measures for a morphology.
 * @param params - list of params read from configuration file.
 */

void Measures::measurePhenotype(int argc, char* argv[],std::map<std::string, double> params, int generation){

    std::cout<<" --- "<<this->gen->getId()<<" measuring..."<<std::endl;
    int size = params["size_component"] + params["spacing"]; // size of the component plus the spacing between components

    DecodedGeneticString::Vertex * c = NULL;
    c = this->gen->getDgs().getRoot(); // root for the graph which logically represents the morphology

    this->initalizeMeasures(); // creates map with keys for measures as zero-valued

    this->measureComponent( "bottom","root", c,c, params); // roams graph calculating all measures


    // calculates number of effective joints per total of limbs
    this->gen->updateMeasure("joints_per_limb", roundf((this->gen->getMeasures()["effective_joints"] / (double)this->gen->getMeasures()["connectivity1"])*100)/100);

    // total of all types of joint
    int joints = this->gen->getMeasures()["total_fixed_joints_horizontal"] + this->gen->getMeasures()["total_passive_joints_horizontal"] + this->gen->getMeasures()["total_active_joints_horizontal"] + this->gen->getMeasures()["total_fixed_joints_vertical"] + this->gen->getMeasures()["total_passive_joints_vertical"] + this->gen->getMeasures()["total_active_joints_vertical"];

    if(joints > 0) {

        // proportion of effective-joints in the total of joints
        this->gen->updateMeasure("effective_joints", roundf( (this->gen->getMeasures()["effective_joints"] / (double)joints)*100)/100);

        if(this->gen->getMeasures()["effective_ap_h_joints"] > 0) { // if there is any horizontal active/passive joint

            this->gen->updateMeasure("viable_joints",
                    roundf((this->gen->getMeasures()["viable_joints"] / (double)this->gen->getMeasures()["effective_ap_h_joints"]) * 100) /
                    100 ); // proportion of viable joints in effective horizontal- active/passive - joints
        }
    }

    // general total amount of components: core + bricks + joints
    this->gen->updateMeasure("total_components", 1 + this->gen->getMeasures()["total_bricks"] + joints);


    // transforms totals in percentages for joints and connectivity

    this->gen->updateMeasure("total_bricks", roundf(( this->gen->getMeasures()["total_bricks"] / (double)this->gen->getMeasures()["total_components"])*100)/100 );

    this->gen->updateMeasure("total_fixed_joints_horizontal", roundf((this->gen->getMeasures()["total_fixed_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("total_passive_joints_horizontal", roundf((this->gen->getMeasures()["total_passive_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("total_active_joints_horizontal", roundf((this->gen->getMeasures()["total_active_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);

    this->gen->updateMeasure("total_fixed_joints_vertical", roundf((this->gen->getMeasures()["total_fixed_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("total_passive_joints_vertical", roundf((this->gen->getMeasures()["total_passive_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("total_active_joints_vertical", roundf((this->gen->getMeasures()["total_active_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);

    this->gen->updateMeasure("connectivity1", roundf ((this->gen->getMeasures()["connectivity1"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("connectivity2", roundf ((this->gen->getMeasures()["connectivity2"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("connectivity3", roundf ((this->gen->getMeasures()["connectivity3"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    this->gen->updateMeasure("connectivity4", roundf ((this->gen->getMeasures()["connectivity4"] / (double)this->gen->getMeasures()["total_components"])*100)/100);

    // calculates the length ratio

    int max_x = 0;
    int max_y = 0;
    int min_x = 0;
    int min_y = 0;

    for( const auto& iter : this->gen->getList_components() ){

        // finds out the values for the extreme x/y coordinates
        if (iter.first.first > max_x){ max_x = iter.first.first; }
        if (iter.first.first < min_x){ min_x = iter.first.first; }
        if (iter.first.second > max_y){ max_y = iter.first.second; }
        if (iter.first.second < min_y){ min_y = iter.first.second; }

    }

    int horizontal_length = max_x + size - min_x ; // horizontal length, considering the components in the very left/right extremes
    int vertical_length = max_y + size - min_y;   // vertical, length considering the components in the very top/bottom extremes

    if( horizontal_length < vertical_length ) { // proportion of the shortest length for the longest
        this->gen->updateMeasure("length_ratio", roundf(  (horizontal_length / (double)vertical_length) * 100) / 100);
    }else{
        this->gen->updateMeasure("length_ratio", roundf(  (vertical_length / (double)horizontal_length) * 100) / 100);
    }


    // calculates center of mass of the body

    double PI = 3.14159265;
    int avg_x = 0;
    int avg_y = 0;
    std::vector<int> x = std::vector<int>();
    std::vector<int> y = std::vector<int>();
    std::vector<double> a = std::vector<double>();

    for( const auto& iter : this->points ){
        avg_x += iter.first.first;
        avg_y += iter.first.second;

        x.push_back(iter.first.first);
        y.push_back(iter.first.second);
    }
    avg_x = avg_x / (double)this->points.size();
    avg_y = avg_y / (double)this->points.size();

    std::cout<<" >>mass x "<<avg_x<<" y "<<avg_y<<std::endl;

    // orders the set of points clockwise

    for( const auto& iter : this->points ) {
        double angle = atan2(iter.first.second - avg_y, iter.first.first - avg_x)* 180 / PI;
        a.push_back(angle);
        this->points[std::make_pair(iter.first.first,iter.first.second)] = angle;
    }

    int temp = 0;
    for(int i = 0 ; i < x.size() ; i++) {
        for(int j = i+1 ; j < x.size() ; j++) {

            if(a[i] > a[j]) {

                temp = a[j] ;
                a[j] = a[i] ;
                a[i] = temp ;

                temp = x[j] ;
                x[j] = x[i] ;
                x[i] = temp ;

                temp = y[j] ;
                y[j] = y[i] ;
                y[i] = temp ;
            }
        }
    }


    for(int i = 0 ; i < x.size() ; i++) {

        std::cout<<"point: a "<<a[i]<<"  x " <<x[i]<<" y "<<y[i]<<std::endl;
    }



    // calculates the covered area

    // number of components which would fit in the area calculated by the verified lengths
    int expected_components =  (horizontal_length / size) * (vertical_length / size);

    // actual number of components per expected number of components
    this->gen->updateMeasure("coverage", roundf( (this->gen->getMeasures()["total_components"] / (double)expected_components)*100)/100);


    // calculates the average distance among components
    int comps = 0;
    for( const auto& iter1 : this->gen->getList_components() ){

        int comp = 0;
        for( const auto& iter2 : this->gen->getList_components() ){
                // sum of distances from the component to all the other components
                comp += abs( iter1.first.first - iter2.first.first) + abs( iter1.first.second - iter2.first.second);
        }

        comp /= this->gen->getList_components().size() - 1; // average of the distances for each component
        comps += comp;

    }

    comps /= this->gen->getList_components().size();  // average of all the averages
    // normalizes according to the number of components
    this->gen->updateMeasure("spreadness", roundf((comps / (double)this->gen->getMeasures()["total_components"])*100)/100);




    // calculates the horizontal symmetry
    int ncomp = 0;
    for( const auto& it : this->gen->getList_components() ){
        if( it.first.first < 0 or it.first.first >= size ) {
            ncomp +=1;
            std::pair<int, int> l_key = std::make_pair(it.first.first * (-1),
                                                       it.first.second); // the horizontal-opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if (l_it != this->gen->getList_components().end()) {

                if (it.second == this->gen->getList_components()[l_key]) { // if the component at the other side is the same type
                    this->gen->updateMeasure("horizontal_symmetry", this->gen->getMeasures()["horizontal_symmetry"] + 1 );
                }
            }
        }
    }
    if(ncomp > 0){ this->gen->updateMeasure("horizontal_symmetry" , roundf( (this->gen->getMeasures()["horizontal_symmetry"] / ncomp)*100)/100); }else{ this->gen->updateMeasure("horizontal_symmetry", 1); }


    // calculates the vertical symmetry
    ncomp = 0;
    for( const auto& it : this->gen->getList_components() ){
        if(it.first.second < 0 or it.first.second >= size ) {
            ncomp +=1;
            std::pair<int, int> l_key = std::make_pair(it.first.first,
                                                       it.first.second * (-1)); // the vertical-opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if (l_it != this->gen->getList_components().end()) {

                if (it.second == this->gen->getList_components()[l_key]) { // if the component at the other side is the same type
                    this->gen->updateMeasure("vertical_symmetry", this->gen->getMeasures()["vertical_symmetry"]+ 1 );
                }
            }
        }
    }
    if(ncomp > 0){ this->gen->updateMeasure("vertical_symmetry", roundf( (this->gen->getMeasures()["vertical_symmetry"] / ncomp)*100 )/100); }else{ this->gen->updateMeasure("vertical_symmetry", 1); }



    // exports measures to file (individual and populational)

    std::ofstream measures_file_general;
    std::string path = "../../tests/measures.txt";
    measures_file_general.open(path, std::ofstream::app);

    std::ofstream measures_file;
    path = "../../tests/measures"+this->gen->getId()+"_g"+ std::to_string(generation)+".txt";
    measures_file.open(path);
    measures_file_general << this->gen->getId();


    for( const auto& mea : this->gen->getMeasures() ){
        measures_file << mea.first << " : " << mea.second << std::endl;
        measures_file_general <<"\t"<< mea.second;
    }
    measures_file_general << std::endl;

    measures_file.close();
    measures_file_general.close();


}



/**
 * Roams the graph of components calculating measures of the morphology.
 * @param reference - reference of origin-side for the turtle
 * @param direction - direction to which to add the current component, relative to the previous component
 * @param c1 - pointer to the root
 * @param c2 - pointer to the current item
 * @param params - list of params read from configuration file.
 */

void Measures::measureComponent( std::string reference, std::string direction, DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2, std::map<std::string, double> params) {

    if (c2 != NULL) { // condition to stop the recursion


        //  calculates amounts of types of components

        if (c2->item == "BNNN") {
            this->gen->updateMeasure("total_bricks", this->gen->getMeasures()["total_bricks"]+1);
        } // counts for each brick

        if (c2->item == "J1") {
            this->gen->updateMeasure("total_fixed_joints_horizontal", this->gen->getMeasures()["total_fixed_joints_horizontal"]+1);
        } // counts for each horizontal fixed joint

        if (c2->item == "PJ1") {
            this->gen->updateMeasure("total_passive_joints_horizontal", this->gen->getMeasures()["total_passive_joints_horizontal"]+1);
        } // counts for each horizontal passive joint

        if (c2->item == "AJ1") {
            this->gen->updateMeasure("total_active_joints_horizontal", this->gen->getMeasures()["total_active_joints_horizontal"]+1);
        } // counts for each horizontal active joint

        if (c2->item == "J2") {
            this->gen->updateMeasure("total_fixed_joints_vertical", this->gen->getMeasures()["total_fixed_joints_vertical"]+1);
        } // counts for each vertical fixed joint

        if (c2->item == "PJ2") {
            this->gen->updateMeasure("total_passive_joints_vertical", this->gen->getMeasures()["total_passive_joints_vertical"]+1);
        } // counts for each vertical passive joint

        if (c2->item == "AJ2")
        {  this->gen->updateMeasure("total_active_joints_vertical", this->gen->getMeasures()["total_active_joints_vertical"]+1);
        } // counts for each vertical active joint



        //  calculates connectivity (how many connected sides a component has)

        int connected_sides = 0;
        if (c1 != c2) {
            connected_sides++; // if a component is not the core, it is always connected to a parent
        } else {
            if (c2->back != NULL) { connected_sides++; } // if there is connection on the back of the core component
        }

        if (c2->left != NULL) { connected_sides++; } //  if there is left-side connectivity
        if (c2->right != NULL) { connected_sides++; } // if there is right-side connectivity
        if (c2->front != NULL) { connected_sides++; } //  if there is front-side connectivity

        if (connected_sides == 1) {
            this->gen->updateMeasure("connectivity1", this->gen->getMeasures()["connectivity1"]+1);
        } // counts for: one side is connected

        if (connected_sides == 2) { // if both sides have connections
            this->gen->updateMeasure("connectivity2", this->gen->getMeasures()["connectivity2"]+1); // counts for: two sides are connected

            // if item is a joint and is connected to brick/core
            if ((c2->item.substr(0, 1) == "J" or c2->item.substr(0, 2) == "PJ" or c2->item.substr(0, 2) == "AJ") and
                (c2->back->item == "CNNN" or c2->back->item == "BNNN") and (c2->front->item == "BNNN")) {

                this->gen->updateMeasure("effective_joints", this->gen->getMeasures()["effective_joints"]+1);
            } // counts for joints effective joints: joints connected by both sides to brick/core component


            if ( (c2->item == "AJ1" or c2->item == "PJ1" ) and 
            (c2->back->item == "CNNN" or c2->back->item == "BNNN") and (c2->front->item == "BNNN")) { 

                this->gen->updateMeasure("effective_ap_h_joints", this->gen->getMeasures()["effective_ap_h_joints"]+1); 
            } // counts for horizontal- active/passive - joints connected by both sides to brick or core component (effective)
        }

        if (connected_sides == 3) {
            this->gen->updateMeasure("connectivity3", this->gen->getMeasures()["connectivity3"]+1);
        } // counts for: three sides are connected

        if (connected_sides == 4) {
            this->gen->updateMeasure("connectivity4", this->gen->getMeasures()["connectivity4"]+1);
        } // counts for: all four sides are connected




        // calculates viable (no neighbours preventing movement) horizontal- passive/active joints
        int size = (int) params["size_component"] + params["spacing"];

        if (c2 != c1) {


            // finds out the x/y coordinates of both supposed neighbours of the joint: l - neighbour on the left / r - neighbour on the right

            int coor_x_l = 0;
            int coor_x_r = 0;
            int coor_y_l = 0;
            int coor_y_r = 0;

            if (direction == "left") { // if current element was added on the left of his parent

                if (reference == "bottom") {   // reference is according to the turtle path, starting in the bottom
                    // calculates coordinates for a supposed neighboard on the left of the joint
                    coor_x_l = c2->x;
                    coor_y_l = c2->y + size;
                    // calculates coordinates for a supposed neighboard on the right of the joint
                    coor_x_r = c2->x;
                    coor_y_r = c2->y - size;
                    reference = "rside";

                    // finds out outlining points of the component

                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x, c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x, c2->y), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l + size, coor_y_l); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r + size, coor_y_r); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size, c2->y + size), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x + size, c2->y ), 0);  } // if  the supposed right angle was not found

                } else if (reference == "top") {
                    coor_x_l = c2->x;
                    coor_y_l = c2->y - size;
                    coor_x_r = c2->x;
                    coor_y_r = c2->y + size;
                    reference = "lside";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x + size, c2->y), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x + size, c2->y + size), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l - size, coor_y_l); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r - size, coor_y_r); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and  this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y ), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y + size ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "lside") {
                    coor_x_l = c2->x - size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x + size;
                    coor_y_r = c2->y;
                    reference = "bottom";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x, c2->y), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x + size, c2->y ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l , coor_y_l + size); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r , coor_y_r + size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y + size ), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x +size , c2->y + size ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "rside") {
                    coor_x_l = c2->x + size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x - size;
                    coor_y_r = c2->y;
                    reference = "top";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x +size, c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x , c2->y + size), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l , coor_y_l - size); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r , coor_y_r - size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x +size , c2->y), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y), 0);  } // if  the supposed right angle was not found

                }
            }
            if (direction == "right") {

                if (reference == "bottom") {
                    coor_x_l = c2->x;
                    coor_y_l = c2->y - size;
                    coor_x_r = c2->x;
                    coor_y_r = c2->y + size;
                    reference = "lside";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x +size, c2->y ), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x + size , c2->y + size), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l - size , coor_y_l  ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r - size , coor_y_r ); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x  , c2->y), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL  and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y +size), 0);  } // if  the supposed right angle was not found


                } else if (reference == "top") {
                    coor_x_l = c2->x;
                    coor_y_l = c2->y - size;
                    coor_x_r = c2->x;
                    coor_y_r = c2->y + size;
                    reference = "rside";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x , c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x  , c2->y ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l + size , coor_y_l  ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r + size , coor_y_r ); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size  , c2->y + size), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x +size, c2->y ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "lside") {
                    coor_x_l = c2->x + size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x - size;
                    coor_y_r = c2->y;
                    reference = "top";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x +size , c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x  , c2->y+size ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l  , coor_y_l - size ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r  , coor_y_r - size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size  , c2->y ), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y  ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "rside") {
                    coor_x_l = c2->x - size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x + size;
                    coor_y_r = c2->y;
                    reference = "bottom";

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x  , c2->y ), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x +size , c2->y ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l  , coor_y_l + size ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r  , coor_y_r + size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x  , c2->y +size), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x +size, c2->y +size ), 0);  } // if  the supposed right angle was not found


                }
            }
            if (direction == "front") {

                if (reference == "bottom") {
                    coor_x_l = c2->x - size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x + size;
                    coor_y_r = c2->y;

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x  , c2->y ), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x +size , c2->y ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l  , coor_y_l + size ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r  , coor_y_r + size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x  , c2->y +size), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x +size, c2->y +size ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "top") {
                    coor_x_l = c2->x + size;
                    coor_y_l = c2->y;
                    coor_x_r = c2->x - size;
                    coor_y_r = c2->y;

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x +size , c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x  , c2->y+size ), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l  , coor_y_l - size ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r  , coor_y_r - size); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size  , c2->y ), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y  ), 0);  } // if  the supposed right angle was not found

                } else if (reference == "lside") {
                    coor_x_l = c2->x;
                    coor_y_l = c2->y + size;
                    coor_x_r = c2->x;
                    coor_y_r = c2->y - size;

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x +size, c2->y ), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x + size , c2->y + size), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l -size , coor_y_l  ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r -size , coor_y_r ); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x  , c2->y), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y +size ), 0);  } // if  the supposed right angle was not found


                } else if (reference == "rside") {
                    coor_x_l = c2->x;
                    coor_y_l = c2->y + size;
                    coor_x_r = c2->x;
                    coor_y_r = c2->y -size;

                    // finds out outlining points of the component
                    if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x, c2->y + size), 0); }
                    if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x, c2->y), 0); }
                    std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l +size , coor_y_l  ); // coordinates for a supposed angle on the left of the component
                    std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r +size , coor_y_r ); // coordinates for a supposed angle on the right of the component
                    if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size, c2->y + size), 0); } // if  the supposed left angle was not found
                    if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x + size, c2->y ), 0);  } // if  the supposed right angle was not found


                }
            }
            if (direction == "root-back") {
                coor_x_l = c2->x + size;
                coor_y_l = c2->y;
                coor_x_r = c2->x - size;
                coor_y_r = c2->y;
                reference = "top";

                // finds out outlining points of the component
                if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x + size, c2->y + size), 0); }
                if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x, c2->y + size), 0); }
                std::pair<int, int> coor_ang_l_key = std::make_pair(coor_x_l  , coor_y_l - size  ); // coordinates for a supposed angle on the left of the component
                std::pair<int, int> coor_ang_r_key = std::make_pair(coor_x_r  , coor_y_r - size); // coordinates for a supposed angle on the right of the component
                if (c2->left == NULL and this->gen->getList_components().count(coor_ang_l_key)>0) { this->points.emplace( std::make_pair(c2->x + size, c2->y), 0); } // if  the supposed left angle was not found
                if (c2->right == NULL and this->gen->getList_components().count(coor_ang_r_key)>0) { this->points.emplace( std::make_pair(c2->x , c2->y ), 0);  } // if  the supposed right angle was not found


            }

            std::pair<int, int> coor_l_key = std::make_pair(coor_x_l, coor_y_l); // coordinates for a supposed neighboard on the left of the joint
            std::pair<int, int> coor_r_key = std::make_pair(coor_x_r, coor_y_r); // coordinates for a supposed neighboard on the right of the joint

            if(this->gen->getList_components().count(coor_l_key)<=0 and this->gen->getList_components().count(coor_r_key)<=0){  // if none the supposed neighboards were found

                if (connected_sides == 2) { // if the joint is connect at both sides

                    if ((c2->item == "PJ1" or c2->item == "AJ1") and (c2->back->item == "CNNN" or c2->back->item == "BNNN") and
                        (c2->front->item == "BNNN")) { // if it is an effective joint

                        this->gen->updateMeasure("viable_joints", this->gen->getMeasures()["viable_joints"] + 1);
                    }
                }
            }

        }else{
            // finds out outlining points of the component
            if (c2->left == NULL and c2->front == NULL) {           this->points.emplace( std::make_pair(c2->x, c2->y ), 0); }
            if (c2->right == NULL and c2->front == NULL) {          this->points.emplace( std::make_pair(c2->x + size, c2->y), 0); }
            if (c2->left == NULL and c2->back == NULL) {            this->points.emplace( std::make_pair(c2->x , c2->y + size), 0); }
            if (c2->right == NULL and c2->back == NULL) {    this->points.emplace( std::make_pair(c2->x + size, c2->y + size), 0); }
        }


        // recursively calls this function to roam the rest of the graphical because it was in violation
        this->measureComponent(reference, "left", c1, c2->left, params);
        this->measureComponent(reference, "front", c1, c2->front, params);
        this->measureComponent(reference, "right", c1, c2->right, params);
        if (c2 == c1) {
            this->measureComponent(reference, "root-back", c1, c2->back, params);
        }
    }


}

/**
 * Adds and initializes items in the array of measures of the morphology of a genome.
 */

void Measures::initalizeMeasures(){

    this->gen->updateMeasure("total_components", 0); //  total amount of components of all types  in the body
    this->gen->updateMeasure("total_bricks", 0); //  total amount of brick-components
    this->gen->updateMeasure("total_fixed_joints_horizontal", 0); //  total amount of horizontal fixed-joint-components
    this->gen->updateMeasure("total_passive_joints_horizontal", 0); // measure: total amount of horizontal passive-joint-components
    this->gen->updateMeasure("total_active_joints_horizontal", 0); //   total amount of horizontal active-joint-components
    this->gen->updateMeasure("total_fixed_joints_vertical", 0); //  total amount of vertical fixed-joint-components
    this->gen->updateMeasure("total_passive_joints_vertical", 0); // measure: total amount of vertical passive-joint-components
    this->gen->updateMeasure("total_active_joints_vertical", 0); //   total amount of vertical active-joint-components
    this->gen->updateMeasure("connectivity1", 0); //   total of components with one side connected to another component
    this->gen->updateMeasure("connectivity2",0); //  total of components with two sides connected to another component
    this->gen->updateMeasure("connectivity3", 0); // total of components with three sides connected to another component
    this->gen->updateMeasure("connectivity4", 0); //   total of components with four sides connected to another component
    this->gen->updateMeasure("effective_joints", 0); //  total of joints connected by both sides to a brick or core component
    this->gen->updateMeasure("effective_ap_h_joints", 0); //  total of horizontal- active/passive- joints connected by both sides to a brick or core component 
    this->gen->updateMeasure("viable_joints", 0); //  total of effective joints which have no neighboards preventing movement
    this->gen->updateMeasure("length_ratio", 0); // length of the shortest side dived by the longest
    this->gen->updateMeasure("coverage", 0); // proportion of the expected area (given the horizontal e vertical lengths) that is covered with components
    this->gen->updateMeasure("spreadness", 0); // average distance of each component from each other in the axises x/y
    this->gen->updateMeasure("horizontal_symmetry",  0); // proportion of components in the left side which match with the same type of component in the same relative position on the right side
    this->gen->updateMeasure("vertical_symmetry", 0); // proportion of components in the top side which match with the same type of component in the same relative position on the bottom side
    this->gen->updateMeasure("joints_per_limb", 0); //  total amount of effective joints per limb
}




