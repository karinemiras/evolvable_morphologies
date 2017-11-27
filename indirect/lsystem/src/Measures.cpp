//
// Created by Karine Miras on 12/04/2017.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>
#include <math.h>
#include <random>
#include <sstream>

#include "Genome.h"
#include "Measures.h"
#include "LSystem.h"
#include "Tests.h"


/**
 * Sets a variable with the genome for which the measures will be calculated.
 * @param gen - genome
 */
void Measures::setGenome(Genome &gen) {
    this->gen = &gen;
}

Genome * Measures::getGenome() {
    return this->gen;
}


/**
* Adds and initializes items in the array of measures of the morphology of a genome.
*/

void Measures::initalizeMeasures() {


    this->gen->updateMeasure("total_bricks", 0); //     brick-components
    this->gen->updateMeasure("total_active_joints_horizontal",
                             0); //   total amount of horizontal active-joint-components
    this->gen->updateMeasure("total_active_joints_vertical",
                             0); //    vertical active-joint-components
    this->gen->updateMeasure("coverage",
                             0); // proportion of the expected area (given the horizontal e vertical lengths) that is covered with components
    this->gen->updateMeasure("horizontal_symmetry",
                             0); // proportion of components in the left side which match with the same type of component in the same relative position on the right side
    this->gen->updateMeasure("vertical_symmetry",
                             0); // proportion of components in the top side which match with the same type of component in the same relative position on the bottom side
    this->gen->updateMeasure("connectivity2",
                             0); //    components with two sides connected to another component
    this->gen->updateMeasure("connectivity4",
                             0); //   components with four sides connected to another component
    this->gen->updateMeasure("effective_joints",
                             0); //   joints connected by both sides to a brick or core component
    this->gen->updateMeasure("connectivity1",
                             0); //    components with one side connected to another component
    this->gen->updateMeasure("symmetry",
                             0); //   maximum of horizontal and vertical symmetry
    this->gen->updateMeasure("total_components",
                             0); //  proportion of components of all types  in the body given the maximum possible size
    this->gen->updateMeasure("length_ratio",
                             0); // length of the shortest side dived by the longest
    this->gen->updateMeasure("branching",
                             0); //  connectivity4,  divided by the total ofcomponents
    this->gen->updateMeasure("sensors_slots",
                             0); // number of sensors per slot
    this->gen->updateMeasure("sensors",
                             0); // number of sensors
}


/**
 * Calculates several measures for a morphology.
 * @param params - list of params read from configuration file.
 * @param dirpath - directory where file with measures will be saved.
 */
void Measures::measurePhenotype(std::map<std::string, double> params,
                                std::string dirpath, int generation) {


    // size of the component plus the spacing between components
    int size = (int) (params["size_component"] + params["spacing"]);

    // root for the graph which logically represents the morphology (phenotype)
    DecodedGeneticString::Vertex *root = NULL;
    root = this->gen->getDgs().getRoot();

    // creates map with keys for measures as zero-valued items
    this->initalizeMeasures();

    // roams graph calculating all measures
    this->measureComponent("bottom", "root", root, root, params);


    /* BEGINNING:  calculating measures  */

    // total of all types of joints
    int joints = (int) (
            0
            + this->gen->getMeasures()["total_active_joints_horizontal"]
            + this->gen->getMeasures()["total_active_joints_vertical"]);

    // total amount of components of all types: core + bricks + joints
    this->gen->updateMeasure("total_components",
                             1
                             + this->gen->getMeasures()["total_bricks"]
                             + joints);


    // total of effective joints: joints with both sides connected to core/brick
    this->gen->updateMeasure("effective_joints",
                             this->gen->getMeasures()["effective_joints"]
                             +
                             this->gen->getMeasures()["viable_horizontal_joints"]);

    // practical limits for effective joints
    int limit_joints = std::trunc(
            (this->gen->getMeasures()["total_components"] - 1) / 2);


    // normalizes the number of effective joints given a practical limit
    if (limit_joints > 0) {
        this->gen->updateMeasure("effective_joints",
                                 roundf(
                                         ((float) this->gen->getMeasures()["effective_joints"]
                                          / (float) limit_joints)
                                         * 1000) / 1000);
    }

    int limit_limbs = 0;
    // normalizes the number of limbs per total of components
    if (this->gen->getMeasures()["total_components"] <= 5) {
        limit_limbs = this->gen->getMeasures()["total_components"] - 1;
    } else {
        limit_limbs = 2 *
                      floor((this->gen->getMeasures()["total_components"] - 6) /
                            3)
                      + fmod((this->gen->getMeasures()["total_components"] - 6),
                             3)
                      + 4;
    }

    if (limit_limbs > 0) {
        this->gen->updateMeasure("connectivity1",
                                 roundf(
                                         ((float) this->gen->getMeasures()["connectivity1"]
                                          / (float) limit_limbs)
                                         * 1000) / 1000);
    }

    int limit_connectivity2 = this->gen->getMeasures()["total_components"] -
                              2; // disccounts bothsidelimbs and head
    if (limit_connectivity2 > 0) {
        this->gen->updateMeasure("connectivity2",
                                 roundf(
                                         ((float) this->gen->getMeasures()["connectivity2"]
                                          / (float) limit_connectivity2)
                                         * 1000) / 1000);
    }


    // normalizes branching given a practical limit
    int limit_branching = std::trunc(
            (this->gen->getMeasures()["total_components"] - 2) /
            3); // discounts 2 compomnents on the borders, and divides by the central triads
    if (limit_branching > 0) {
        this->gen->updateMeasure("branching",
                                 roundf(
                                         (float) (
                                                 this->gen->getMeasures()["connectivity4"]
                                                 / (float) limit_branching)
                                         * 1000) / 1000);
    }


    /* BEGINNING:  length ratio: shortest length by longest length  */

    int max_x = 0, max_y = 0, min_x = 0, min_y = 0;

    for (const auto &iter : this->gen->getList_components()) { // for each component of the
        // morphology

        // finds out the values for its extreme x/y coordinates
        if (iter.first.first > max_x) { max_x = iter.first.first; }
        if (iter.first.first < min_x) { min_x = iter.first.first; }
        if (iter.first.second > max_y) { max_y = iter.first.second; }
        if (iter.first.second < min_y) { min_y = iter.first.second; }

    }

    int horizontal_length = max_x + size -
                            min_x; // horizontal length, considering the components in the very left/right extremes
    int vertical_length = max_y + size -
                          min_y;   // vertical, length considering the components in the very top/bottom extremes

    if (horizontal_length <
        vertical_length) { // proportion of the shortest length for the longest
        this->gen->updateMeasure("length_ratio", roundf(
                (horizontal_length / (float) vertical_length)
                * 1000) / 1000);
    } else {
        this->gen->updateMeasure("length_ratio", roundf(
                (vertical_length / (float) horizontal_length)
                * 1000) / 1000);
    }

    /* END:  length ratio: shortest length by longest length  */

    // horizontal symmetry
    int ncomp = 0;
    for (const auto &it : this->gen->getList_components()) {

        int x = it.first.first;
        int y = it.first.second;
        if (x < 0 or x >=
                     size) { // if component is not on horizontal the spine of the morphology
            ncomp += 1;
            std::pair<int, int> l_key = std::make_pair(x * (-1),
                                                       y); // the horizontal-opposite coordinate

            // if any component is found in the opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if (this->gen->getList_components().count(l_key) > 0) {

                this->gen->updateMeasure("horizontal_symmetry",
                                         this->gen->getMeasures()["horizontal_symmetry"] +
                                         1);
            }
        }
    }
    if (ncomp > 0) {
        this->gen->updateMeasure("horizontal_symmetry",
                                 roundf((
                                                (float) this->gen->getMeasures()
                                                ["horizontal_symmetry"]
                                                / (float) ncomp) * 1000) /
                                 1000);
    }


    // vertical symmetry
    ncomp = 0;
    for (const auto &it : this->gen->getList_components()) {

        int x = it.first.first;
        int y = it.first.second;
        if (y < 0 or y >=
                     size) { // if component is not on vertical the spine of the morphology
            ncomp += 1;
            std::pair<int, int> l_key = std::make_pair(x, y *
                                                          (-1)); // the vertical-opposite coordinate

            // if any component is found in the opposite coordinate
            auto l_it = this->gen->getList_components().find(l_key);
            if (this->gen->getList_components().count(l_key) > 0) {

                this->gen->updateMeasure("vertical_symmetry",
                                         this->gen->getMeasures()["vertical_symmetry"] +
                                         1);
            }
        }
    }
    if (ncomp > 0) {
        this->gen->updateMeasure("vertical_symmetry",
                                 roundf((
                                                (float) this->gen->getMeasures()["vertical_symmetry"]
                                                / (float) ncomp) * 1000) /
                                 1000);
    }


    // selects the maximum symmetry
    this->gen->updateMeasure("symmetry",
                             std::max(
                                     this->gen->getMeasures()["vertical_symmetry"],
                                     this->gen->getMeasures()["horizontal_symmetry"]));

    /* END:   symmetry */


    /* BEGINNING: covered area  */

    // number of components which would fit in the area calculated given the verified lengths
    int expected_components =
            (horizontal_length / size) * (vertical_length / size);

    // actual number of components per expected number of components
    this->gen->updateMeasure("coverage",
                             roundf(
                                     ((float) this->gen->getMeasures()["total_components"]
                                      / (float) expected_components)
                                     * 1000) / 1000);

    /* END: covered area  */


    // normalizes total number of components by maximum possible number of components
    this->gen->updateMeasure("total_components",
                             this->gen->getMeasures()["total_components"] /
                             params["max_comps"]);

    this->gen->updateMeasure("sensors",
                                 this->gen->getMeasures()["sensors"]
                                 / this->gen->getMeasures()["sensors_slots"]);


    /* END:  calculating measures  */


    // checks validity of morphology
    if ( (this->gen->getMeasures()["total_active_joints_horizontal"] == 0
          and this->gen->getMeasures()["total_active_joints_vertical"] == 0)
        or this->gen->getMeasures()["sensors"] == 0)
    { // if there is no sensor or no joint, morphology is invalid
        this->gen->setValid(0);
    }


    // removes temporary measures

    this->gen->removeMeasure("total_bricks");
    this->gen->removeMeasure("total_active_joints_horizontal");
    this->gen->removeMeasure("total_active_joints_vertical");
    this->gen->removeMeasure("connectivity4");
    this->gen->removeMeasure("horizontal_symmetry");
    this->gen->removeMeasure("vertical_symmetry");
    this->gen->removeMeasure("sensors_slots");


    // #TEST: checks if there is any measure ouy of the expected range
    Tests tests = Tests(this->experiment_name, this->params);
    tests.testMeasures(this->gen->getId(), this->gen->getMeasures());


    /* BEGINNING: exports measures to files */

    std::ofstream measures_file_general;
    std::string path =
            "../../experiments/" + this->experiment_name + "/measures.txt";
    measures_file_general.open(path, std::ofstream::app);

    std::ofstream measures_file;
    path = "../../experiments/" + this->experiment_name + dirpath +
           std::to_string(generation) + "/measures" + this->gen->getId() +
           ".txt";
    measures_file.open(path);

    measures_file_general << std::to_string(generation);
    measures_file_general << " " << this->gen->getId();

    for (const auto &mea : this->gen->getMeasures()) {

        measures_file << mea.first << ":" << mea.second << std::endl;
        measures_file_general << " " << mea.second;
    }

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

void Measures::measureComponent(std::string reference,
                                std::string direction,
                                DecodedGeneticString::Vertex *c1,
                                DecodedGeneticString::Vertex *c2,
                                std::map<std::string, double> params) {


    if (c2 != NULL) { // condition to stop the recursion


        /* BEGINNING: number of types of components  */

        if (c2->item == "B") {
            this->gen->updateMeasure("total_bricks",
                                     this->gen->getMeasures()["total_bricks"] +
                                     1);
        } // counts for each brick

        if (c2->item == "AJ1") {
            this->gen->updateMeasure("total_active_joints_horizontal",
                                     this->gen->getMeasures()["total_active_joints_horizontal"] +
                                     1);
        } // counts for each horizontal active joint


        if (c2->item == "AJ2") {
            this->gen->updateMeasure("total_active_joints_vertical",
                                     this->gen->getMeasures()["total_active_joints_vertical"] +
                                     1);
        } // counts for each vertical active joint

        /* END:  calculates number of types of components  */


        /* BEGINNING:   calculates connectivity of slots */

        int connected_sides = 0;
        if (c1 != c2) {
            connected_sides++; // if a component is not the core, it is always connected to a parent
        } else {
            if (c2->back !=
                NULL) { connected_sides++; } // if there is connection on the back of the core component
        }

        if (c2->left !=
            NULL) { connected_sides++; } //  if there is left-side connectivity
        if (c2->right !=
            NULL) { connected_sides++; } // if there is right-side connectivity
        if (c2->front !=
            NULL) { connected_sides++; } //  if there is front-side connectivity


        if(c2->sensor_left != "Sn")
            this->gen->updateMeasure("sensors",
                                     this->gen->getMeasures()["sensors"] + 1);
        if(c2->sensor_front != "Sn")
            this->gen->updateMeasure("sensors",
                                     this->gen->getMeasures()["sensors"] + 1);
        if(c2->sensor_right != "Sn")
            this->gen->updateMeasure("sensors",
                                     this->gen->getMeasures()["sensors"] + 1);
        if(c2->sensor_back != "Sn")
            this->gen->updateMeasure("sensors",
                                     this->gen->getMeasures()["sensors"] + 1);


        if (connected_sides == 0) {
            this->gen->updateMeasure("sensors_slots",
                                     this->gen->getMeasures()["sensors_slots"] +
                                     4);
        }

        if (connected_sides == 1) {
            // not the head
            if (c2->item != "C") { // counts for: one side is connected
                this->gen->updateMeasure("connectivity1",
                                         this->gen->getMeasures()["connectivity1"] +
                                         1);
            }
            if (c2->item == "C" or c2->item == "B")
            {
                this->gen->updateMeasure("sensors_slots",
                                         this->gen->getMeasures()["sensors_slots"] +
                                         3);
            }
        }

        if (connected_sides == 2) {
            // not the head
            if (c2->item != "C") {
                this->gen->updateMeasure("connectivity2",
                                         this->gen->getMeasures()["connectivity2"] +
                                         1); // counts for: two sides are connected
            }
            // if item is a joint (apart from active/passive horizontal ones)
            // and is connected to brick/core
            // counts for joints effective joints: joints connected by both
            // sides to brick/core component
            if ((c2->item == "J1" or c2->item == "J2" or c2->item == "PJ2" or
                 c2->item == "AJ2" or c2->item == "AJ1") and
                (c2->back->item == "C" or c2->back->item == "B") and
                (c2->front->item == "B")) {

                this->gen->updateMeasure("effective_joints",
                                         this->gen->getMeasures()["effective_joints"] +
                                         1);
            }
            if (c2->item == "C" or c2->item == "B")
            {
                this->gen->updateMeasure("sensors_slots",
                                         this->gen->getMeasures()["sensors_slots"] +
                                         2);
            }
        }

        if (connected_sides == 3) { // if three have connections
            if (c2->item == "C" or c2->item == "B")
            {
                this->gen->updateMeasure("sensors_slots",
                                         this->gen->getMeasures()["sensors_slots"] +
                                         1);
            }
        }

        if (connected_sides == 4) {
            this->gen->updateMeasure("connectivity4",
                                     this->gen->getMeasures()["connectivity4"] +
                                     1);
        } // counts for: all four sides are connected

        /* END:   calculates connectivity of slots */


        // recursively calls this function to roam the rest of the graphical
        // because it was in violation
        this->measureComponent(reference, "left", c1, c2->left, params);
        this->measureComponent(reference, "front", c1, c2->front, params);
        this->measureComponent(reference, "right", c1, c2->right, params);
        if (c2 == c1) {
            this->measureComponent(reference, "root-back", c1, c2->back,
                                   params);
        }

    }


}

