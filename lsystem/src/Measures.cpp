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
#include "Tests.h"


/**
 * Sets a variable with the genome for which the measures will be calculated.
 * @param gen - genome
 */
void Measures::setGenome(Genome * gen){
    this->gen = gen;
}

/**
* Adds and initializes items in the array of measures of the morphology of a genome.
*/

void Measures::initalizeMeasures(){


    this->gen->updateMeasure("total_bricks", 0); //     brick-components
    this->gen->updateMeasure("total_fixed_joints_horizontal", 0); //     horizontal fixed-joint-components
    this->gen->updateMeasure("total_passive_joints_horizontal", 0); //   horizontal passive-joint-components
    this->gen->updateMeasure("total_active_joints_horizontal", 0); //   total amount of horizontal active-joint-components
    this->gen->updateMeasure("total_fixed_joints_vertical", 0); //    vertical fixed-joint-components
    this->gen->updateMeasure("total_passive_joints_vertical", 0); //  vertical passive-joint-components
    this->gen->updateMeasure("total_active_joints_vertical", 0); //    vertical active-joint-components
    this->gen->updateMeasure("coverage", 0); // proportion of the expected area (given the horizontal e vertical lengths) that is covered with components
    this->gen->updateMeasure("horizontal_symmetry",  0); // proportion of components in the left side which match with the same type of component in the same relative position on the right side
    this->gen->updateMeasure("vertical_symmetry", 0); // proportion of components in the top side which match with the same type of component in the same relative position on the bottom side
    this->gen->updateMeasure("viable_horizontal_joints", 0); //  total of effective joints which have no neighboards preventing movement
    this->gen->updateMeasure("connectivity2",0); //    components with two sides connected to another component
    this->gen->updateMeasure("connectivity3", 0); //   components with three sides connected to another component
    this->gen->updateMeasure("connectivity4", 0); //   components with four sides connected to another component
    this->gen->updateMeasure("effective_joints", 0); //   joints connected by both sides to a brick or core component
    this->gen->updateMeasure("connectivity1", 0); //    components with one side connected to another component
    this->gen->updateMeasure("symmetry", 0); //   maximum of horizontal and vertical symmetry
    this->gen->updateMeasure("total_components", 0); //  proportion of components of all types  in the body given the maximum possible size
    this->gen->updateMeasure("length_ratio", 0); // length of the shortest side dived by the longest
    this->gen->updateMeasure("sparseness", 0); // average distance of each component from each other in the axises x/y, divided by the total of components
    this->gen->updateMeasure("joints_per_limb", 0); //  proportion of effective joints per limb
    this->gen->updateMeasure("branching",0); // sum of 1*connectivty3 and 2*connectivity4,  divided by the total of components
}


/**
 * Calculates several measures for a morphology.
 * @param params - list of params read from configuration file.
 * @param dirpath - directory where file with measures will be saved.
 */
void Measures::measurePhenotype(std::map<std::string, double> params, std::string dirpath){


    // size of the component plus the spacing between components
    int size = (int) (params["size_component"] + params["spacing"]);

    // root for the graph which logically represents the morphology (phenotype)
    DecodedGeneticString::Vertex * root = NULL;
    root = this->gen->getDgs().getRoot();

    // creates map with keys for measures as zero-valued items
    this->initalizeMeasures();

    // roams graph calculating all measures
    this->measureComponent( "bottom", "root",root ,root , params );


    /* BEGINNING:  calculating measures  */

    // total of all types of joints
    int joints = (int) (     this->gen->getMeasures()["total_fixed_joints_horizontal"]
                           + this->gen->getMeasures()["total_passive_joints_horizontal"]
                           + this->gen->getMeasures()["total_active_joints_horizontal"]
                           + this->gen->getMeasures()["total_fixed_joints_vertical"]
                           + this->gen->getMeasures()["total_passive_joints_vertical"]
                           + this->gen->getMeasures()["total_active_joints_vertical"]);

    // total amount of components of all types: core + bricks + joints
    this->gen->updateMeasure("total_components",
                             1
                             + this->gen->getMeasures()["total_bricks"]
                             + joints );


    // total of effective joints: joints with both sides connected to core/brick
    this->gen->updateMeasure("effective_joints",     this->gen->getMeasures()["effective_joints"]
                                                     + this->gen->getMeasures()["viable_horizontal_joints"]);

    // if there are any limbs
    if( this->gen->getMeasures()["connectivity1"]>0 ) {

        // calculates percentage of number of effective joints per total of limbs
        this->gen->updateMeasure("joints_per_limb",
                                 this->gen->getMeasures()["effective_joints"]
                                 /  (double) this->gen->getMeasures()["connectivity1"]);

        // normalizes per number of components, because there may be more joints than limbs
        this->gen->updateMeasure("joints_per_limb",
                                 roundf(
                                         ( (float) this->gen->getMeasures()["joints_per_limb"]
                                           /   (float) this->gen->getMeasures()["total_components"])
                                         * 100) / 100 );

    }

    // normalizes the number of effective joints per total of components
    this->gen->updateMeasure("effective_joints",
                             roundf(
                                     ( (float) this->gen->getMeasures()["effective_joints"]
                                       /   (float) this->gen->getMeasures()["total_components"])
                                     * 100) / 100 );


    // normalizes the number of limbs per total of components
    this->gen->updateMeasure("connectivity1",
                             roundf (
                                     ( (float) this->gen->getMeasures()["connectivity1"]
                                       / (float) this->gen->getMeasures()["total_components"])
                                     * 100) / 100);


    // accounts for conenctivity in levels 3 (weight 1) and 4 (weight 2)
    this->gen->updateMeasure("branching",
                             roundf (
                                     (   (float) ( this->gen->getMeasures()["connectivity3"] + 2*this->gen->getMeasures()["connectivity4"])
                                       / (float) this->gen->getMeasures()["total_components"])
                                     * 100) / 100);



    /* BEGINNING:  length ratio: shortest length by longest length  */

    int max_x = 0, max_y = 0, min_x = 0, min_y = 0;

    for( const auto& iter : this->gen->getList_components() ){ // for each component of the morphology

        // finds out the values for its extreme x/y coordinates
        if (iter.first.first > max_x){ max_x = iter.first.first; }
        if (iter.first.first < min_x){ min_x = iter.first.first; }
        if (iter.first.second > max_y){ max_y = iter.first.second; }
        if (iter.first.second < min_y){ min_y = iter.first.second; }

    }

    int horizontal_length = max_x + size - min_x ; // horizontal length, considering the components in the very left/right extremes
    int vertical_length = max_y + size - min_y;   // vertical, length considering the components in the very top/bottom extremes

    if( horizontal_length < vertical_length ) { // proportion of the shortest length for the longest
        this->gen->updateMeasure("length_ratio", roundf(
                (horizontal_length / (float) vertical_length)
                * 100) / 100);
    }else{
        this->gen->updateMeasure("length_ratio", roundf(
                (vertical_length / (float) horizontal_length)
                * 100) / 100);
    }

    /* END:  length ratio: shortest length by longest length  */

    /* BEGINNING:   calculates the average distance among components */

    double dist_comps = 0;
    // if there are more components than one
    if( this->gen->getList_components().size() > 1 ) {

        // for each component
        for( const auto& iter1 : this->gen->getList_components() ){

            int dist_comp = 0;
            // compares with all other components
            for( const auto& iter2 : this->gen->getList_components() ){

                // sum of distances from the component to all the other components
                dist_comp += (   abs(iter1.first.first - iter2.first.first)
                                 + abs( iter1.first.second - iter2.first.second)) ;

            }

            // average of the distances for the component
            dist_comp /= this->gen->getList_components().size() - 1;

            dist_comps += dist_comp; // acumulate average distance
        }

        // average of all the averages
        dist_comps /= this->gen->getList_components().size();
    }

    // the division by size is for counting only one uni per component
    dist_comps = (dist_comps / (double) size);

    // normalizes average distance by the number of components
    this->gen->updateMeasure("sparseness",
                             roundf((
                                            (float) dist_comps
                                            / (float) this->gen->getMeasures()["total_components"] )
                                    * 100) / 100);

    /* END:   calculates the average distance among components */

    /* BEGINNING:   symmetry */


    // horizontal symmetry
    int ncomp = 0;
    for( const auto& it : this->gen->getList_components() ){

        int x = it.first.first;
        int y = it.first.second;
        if( x < 0 or x >= size ) { // if component is not on horizontal the spine of the morphology
            ncomp +=1;
            std::pair<int, int> l_key = std::make_pair(x * (-1), y); // the horizontal-opposite coordinate

            // if any component is found in the opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if ( this->gen->getList_components().count(l_key)>0) {

                this->gen->updateMeasure("horizontal_symmetry", this->gen->getMeasures()["horizontal_symmetry"] + 1 ); }
        }
    }
    if(ncomp > 0){ this->gen->updateMeasure("horizontal_symmetry" ,
                                            roundf( (
                                                            (float)this->gen->getMeasures()["horizontal_symmetry"]
                                                            / (float) ncomp)*100)/100); }


    // vertical symmetry
    ncomp = 0;
    for( const auto& it : this->gen->getList_components() ){

        int x = it.first.first;
        int y = it.first.second;
        if(y < 0 or y >= size ) { // if component is not on vertical the spine of the morphology
            ncomp +=1;
            std::pair<int, int> l_key = std::make_pair(x, y * (-1)); // the vertical-opposite coordinate

            // if any component is found in the opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if ( this->gen->getList_components().count(l_key)>0) {

                this->gen->updateMeasure("vertical_symmetry", this->gen->getMeasures()["vertical_symmetry"]+ 1 ); }
        }
    }
    if(ncomp > 0){ this->gen->updateMeasure("vertical_symmetry",
                                            roundf( (
                                                            (float) this->gen->getMeasures()["vertical_symmetry"]
                                                            / (float) ncomp)*100 )/100); }


    // selects the maximum symmetry
    this->gen->updateMeasure("symmetry",
                             std::max(this->gen->getMeasures()["vertical_symmetry"],
                                      this->gen->getMeasures()["horizontal_symmetry"]) );

    /* END:   symmetry */

    // normalizes total number of components by maximum possible number of components
    this->gen->updateMeasure("total_components", this->gen->getMeasures()["total_components"] / params["max_comps"]);


    /* BEGINNING: covered area  */

    // number of components which would fit in the area calculated given the verified lengths
    //        int expected_components =  (horizontal_length / size) * (vertical_length / size);
    //
    //        // actual number of components per expected number of components
    //        this->gen->updateMeasure("coverage",
    //                                            roundf(
    //                                                    (  (float) this->gen->getMeasures()["total_components"]
    //                                                     / (float) expected_components)
    //                                            *100)/100);

    /* END: covered area  */

    //        this->gen->updateMeasure("total_bricks", roundf((  (float) this->gen->getMeasures()["total_bricks"] / (float)this->gen->getMeasures()["total_components"])*100)/100 );
    //        this->gen->updateMeasure("total_fixed_joints_horizontal", roundf((this->gen->getMeasures()["total_fixed_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("total_passive_joints_horizontal", roundf((this->gen->getMeasures()["total_passive_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("total_active_joints_horizontal", roundf((this->gen->getMeasures()["total_active_joints_horizontal"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("total_fixed_joints_vertical", roundf((this->gen->getMeasures()["total_fixed_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("total_passive_joints_vertical", roundf((this->gen->getMeasures()["total_passive_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("total_active_joints_vertical", roundf((this->gen->getMeasures()["total_active_joints_vertical"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("connectivity2", roundf ((this->gen->getMeasures()["connectivity2"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("connectivity3", roundf ((this->gen->getMeasures()["connectivity3"] / (double)this->gen->getMeasures()["total_components"])*100)/100);
    //        this->gen->updateMeasure("connectivity4", roundf ((this->gen->getMeasures()["connectivity4"] / (double)this->gen->getMeasures()["total_components"])*100)/100);

    // calculates center of mass of the body

    //        double PI = 3.14159265;
    //        int avg_x = 0;
    //        int avg_y = 0;
    //        std::vector<int> x = std::vector<int>();
    //        std::vector<int> y = std::vector<int>();
    //        std::vector<double> a = std::vector<double>();
    //
    //        for( const auto& iter : this->points ){
    //            avg_x += iter.first.first;
    //            avg_y += iter.first.second;
    //
    //            x.push_back(iter.first.first);
    //            y.push_back(iter.first.second);
    //        }
    //        avg_x = avg_x / (double)this->points.size();
    //        avg_y = avg_y / (double)this->points.size();

    //std::cout<<" >>mass x "<<avg_x<<" y "<<avg_y<<std::endl;

    // orders the set of points clockwise

    //        for( const auto& iter : this->points ) {
    //            double angle = atan2(iter.first.second - avg_y, iter.first.first - avg_x)* 180 / PI;
    //            a.push_back(angle);
    //            this->points[std::make_pair(iter.first.first,iter.first.second)] = angle;
    //        }
    //
    //        int temp = 0;
    //        for(int i = 0 ; i < x.size() ; i++) {
    //            for(int j = i+1 ; j < x.size() ; j++) {
    //
    //                if(a[i] > a[j]) {
    //
    //                    temp = a[j] ;
    //                    a[j] = a[i] ;
    //                    a[i] = temp ;
    //
    //                    temp = x[j] ;
    //                    x[j] = x[i] ;
    //                    x[i] = temp ;
    //
    //                    temp = y[j] ;
    //                    y[j] = y[i] ;
    //                    y[i] = temp ;
    //                }
    //            }
    //        }
    //
    // for(int i = 0 ; i < x.size() ; i++) {
    //   std::cout<<"point: a "<<a[i]<<"  x " <<x[i]<<" y "<<y[i]<<std::endl;
    // }



    /* END:  calculating measures  */


    // removes temporary measures

    this->gen->removeMeasure("total_bricks");
    this->gen->removeMeasure("total_fixed_joints_horizontal");
    this->gen->removeMeasure("total_passive_joints_horizontal");
    this->gen->removeMeasure("total_active_joints_horizontal");
    this->gen->removeMeasure("total_fixed_joints_vertical");
    this->gen->removeMeasure("total_passive_joints_vertical");
    this->gen->removeMeasure("total_active_joints_vertical");
    this->gen->removeMeasure("connectivity2");
    this->gen->removeMeasure("connectivity3");
    this->gen->removeMeasure("connectivity4");
    this->gen->removeMeasure("viable_horizontal_joints");
    this->gen->removeMeasure("coverage");
    this->gen->removeMeasure("horizontal_symmetry");
    this->gen->removeMeasure("vertical_symmetry");


    // #TEST: checks if there is any measure ouy of the expected range
    Tests tests = Tests();
    tests.testMeasures(this->gen->getId(), this->gen->getMeasures());


    /* BEGINNING: exports measures to files */

    std::ofstream measures_file_general;
    std::string path = "../../experiments/"+dirpath+"/measures.txt";
    measures_file_general.open(path, std::ofstream::app);

    std::ofstream measures_file;
    path = "../../experiments/"+dirpath+"/measures"+this->gen->getId()+".txt";
    measures_file.open(path);
    measures_file_general << this->gen->getId();

    for( const auto& mea : this->gen->getMeasures() ){

        measures_file << mea.first << " : " << mea.second << std::endl;
        measures_file_general <<"\t"<< mea.second;

    }
    std::cout<<std::endl;

    measures_file_general << std::endl;

    measures_file.close();
    measures_file_general.close();

    /* END: exports measures to files */


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


        /* BEGINNING: number of types of components  */

        if (c2->item == "B") {
            this->gen->updateMeasure("total_bricks", this->gen->getMeasures()["total_bricks"]+1);
        } // counts for each brick

        if (c2->item == "J1") {
            this->gen->updateMeasure("total_fixed_joints_horizontal",
                                     this->gen->getMeasures()["total_fixed_joints_horizontal"]+1);
        } // counts for each horizontal fixed joint

        if (c2->item == "PJ1") {
            this->gen->updateMeasure("total_passive_joints_horizontal",
                                     this->gen->getMeasures()["total_passive_joints_horizontal"]+1);
        } // counts for each horizontal passive joint

        if (c2->item == "AJ1") {
            this->gen->updateMeasure("total_active_joints_horizontal",
                                     this->gen->getMeasures()["total_active_joints_horizontal"]+1);
        } // counts for each horizontal active joint

        if (c2->item == "J2") {
            this->gen->updateMeasure("total_fixed_joints_vertical",
                                     this->gen->getMeasures()["total_fixed_joints_vertical"]+1);
        } // counts for each vertical fixed joint

        if (c2->item == "PJ2") {
            this->gen->updateMeasure("total_passive_joints_vertical",
                                     this->gen->getMeasures()["total_passive_joints_vertical"]+1);
        } // counts for each vertical passive joint

        if (c2->item == "AJ2") {
            this->gen->updateMeasure("total_active_joints_vertical",
                                     this->gen->getMeasures()["total_active_joints_vertical"]+1);
        } // counts for each vertical active joint

        /* END:  calculates number of types of components  */


        /* BEGINNING:   calculates connectivity (how many connected sides a component has) / calculates part of the effective joints */

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
            this->gen->updateMeasure("connectivity1",
                                     this->gen->getMeasures()["connectivity1"]+1);
        } // counts for: one side is connected

        if (connected_sides == 2) { // if both sides have connections
            this->gen->updateMeasure("connectivity2",
                                     this->gen->getMeasures()["connectivity2"]+1); // counts for: two sides are connected

            // if item is a joint (apart from active/passive horizontal ones) and is connected to brick/core
            if ((c2->item == "J1" or c2->item == "J2" or c2->item == "PJ2" or c2->item == "AJ2") and
                (c2->back->item == "C" or c2->back->item == "B") and (c2->front->item == "B")) {

                this->gen->updateMeasure("effective_joints",
                                         this->gen->getMeasures()["effective_joints"]+1);
            } // counts for joints effective joints: joints connected by both sides to brick/core component

        }

        if (connected_sides == 3) {
            this->gen->updateMeasure("connectivity3",
                                     this->gen->getMeasures()["connectivity3"]+1);
        } // counts for: three sides are connected

        if (connected_sides == 4) {
            this->gen->updateMeasure("connectivity4",
                                     this->gen->getMeasures()["connectivity4"]+1);
        } // counts for: all four sides are connected

        /* END:   calculates connectivity (how many connected sides a component has) / calculates part of the effective joints */


        /* BEGINNING: calculates viable horizontal passive/active joints (i.e., with no neighbours preventing movement)  */

        int size = (int) (params["size_component"] + params["spacing"]);

        if (c2 != c1) {

            // finds out the x/y coordinates of both supposed neighbours of the joint: l - neighbour on the left / r - neighbour on the right

            int coor_x_l = 0, coor_x_r = 0, coor_y_l = 0, coor_y_r = 0;

            // if current element was added on the left of his parent
            if (direction == "left") {

                // reference is according to the turtle path, starting in the bottom
                if (reference == "bottom") {
                    // calculates coordinates for a supposed neighboard on the left of the joint
                    coor_x_l = c2->x;
                    coor_y_l = c2->y + size;
                    // calculates coordinates for a supposed neighboard on the right of the joint
                    coor_x_r = c2->x;
                    coor_y_r = c2->y - size;
                    reference = "rside";

                    // finds out outlining points of the component

                    if (c2->left == NULL and c2->front == NULL) {
                        this->points.emplace( std::make_pair(c2->x, c2->y + size), 0); }

                    if (c2->right == NULL and c2->front == NULL) {
                        this->points.emplace( std::make_pair(c2->x, c2->y), 0); }

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

                    if ((c2->item == "PJ1" or c2->item == "AJ1") and (c2->back->item == "C" or c2->back->item == "B") and
                        (c2->front->item == "B")) { // if it is an effective joint (both sides conencted)

                        this->gen->updateMeasure("viable_horizontal_joints", this->gen->getMeasures()["viable_horizontal_joints"] + 1);
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

        /* END: calculates viable horizontal passive/active joints (i.e., with no neighbours preventing movement)  */


        // recursively calls this function to roam the rest of the graphical because it was in violation
        this->measureComponent(reference, "left", c1, c2->left, params);
        this->measureComponent(reference, "front", c1, c2->front, params);
        this->measureComponent(reference, "right", c1, c2->right, params);
        if (c2 == c1) {
            this->measureComponent(reference, "root-back", c1, c2->back, params);
        }

    }


}

