//
// Created by Karine Miras on 07/03/2017.
//


#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include <math.h>

#include "Aux.h"
#include "Genome.h"


unsigned int Genome::getTo() {
    //return 4; //test of testing
}

void Genome::setGeneticString(GeneticString * _gs){
    this->gs = _gs;
}

GeneticString * Genome::getGeneticString(){
    return this->gs;
}

std::string Genome::getId(){
    return this->id;
}

std::string Genome::getId_parent1(){
    return this->id_parent1;
}

std::string Genome::getId_parent2(){
    return this->id_parent2;
}

double Genome::getFit_parent1(){
    return this->fit_parent1;
}

double Genome::getFit_parent2(){
    return this->fit_parent2;
}

DecodedGeneticString Genome::getDgs(){
    return this->dgs;
}

std::map< std::string, double > Genome::getMeasures(){
    return this->measures;
};

std::map< std::pair<int, int>, std::string >  Genome::getList_components(){
    return this->list_components;
};

void Genome::updateMeasure(std::string key, double value){
    this->measures[key] = value;
}

void Genome::removeMeasure(std::string key){

    std::map< std::string, double >::iterator it;

    it = this->measures.find(key);
    this->measures.erase(it);

}


void Genome::setGrammar(std::map<std::string, GeneticString *> grammar) {
    this->grammar = grammar;
}

double Genome::getFitness(){
    return this->fitness;
}

/**
* Generates initial production rules for the alphabet.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void Genome::build_grammar(LSystem LS, std::map<std::string, double> params) {

    std::map< std::string, std::string > alp = LS.getAlphabet();
    std::vector<std::string> alp_i = LS.getAlphabetIndex();
    std::vector<std::string> mountingcom = LS.getMountingCommands();
    std::vector<std::string> movingcom = LS.getMovingCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, (int) params["num_initial_comp"]); // distribution for the number of components
    std::uniform_int_distribution<int> dist_2(0, (int) alp_i.size()-1); // distribution for letters of the alphabet
    std::uniform_int_distribution<int> dist_3(0, (int) mountingcom.size()-1); // distribution for the mounting commands
    std::uniform_int_distribution<int> dist_4(0, (int) movingcom.size()-1); // distribution for the moving commands

    // for each letter of the alphabet
    for (std::map< std::string, std::string >::const_iterator it = alp.begin(); it != alp.end(); ++it) {

        std::string letter = it->first;

        std::vector<std::string> letter_items;

        if (letter == "C") { // if it is a core component, forces its inclusion in the rule
            letter_items.push_back(letter);
        }

        // while a raffled number of components is not achieved
        // (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
        while(letter_items.size() < (dist_1(generator)*3) ){

            // raffles a letter to be included
            std::string item = alp_i[dist_2(generator)];

            // prevents core component of being (re)included in the rule
            if (item != "C") {

                // raffles a mounting command to be included
                letter_items.push_back(mountingcom[dist_3(generator)]);
                // adds letter
                letter_items.push_back(item);

                // a raffles a moving command to be included
                letter_items.push_back(movingcom[dist_4(generator)]);

            }
        }

        // build a genetic-string with the the production rule for the letter
        GeneticString * lgs = new GeneticString();
        lgs = this->build_genetic_string(lgs, letter_items);

        // adds letter and its production rule (made a genetic-string) to the grammar of the genome
        this->grammar.emplace(letter, lgs);

    }

}




/**
* Generates initial genomes for the direct encoding.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void Genome::build_genome_direct(LSystem LS, std::map<std::string, double> params) {

    std::vector<std::string> alp_i = LS.getAlphabetIndex();
    std::vector<std::string> mountingcom = LS.getMountingCommands();
    std::vector<std::string> movingcom = LS.getMovingCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, (int) params["max_comps"]); // distribution for the number of components
    // distribution for letters of the alphabet, excluding 0 to prevent core-component of being (re)included in the rule
    std::uniform_int_distribution<int> dist_2(1, (int) alp_i.size()-1);
    std::uniform_int_distribution<int> dist_3(0, (int) mountingcom.size()-1); // distribution for the mounting commands
    std::uniform_int_distribution<int> dist_4(0, (int) movingcom.size()-1); // distribution for the moving commands

    std::vector<std::string> letter_items;

    // forces core-component inclusion in the rule as first item of the genetic string
    letter_items.push_back("C");

    // while a raffled number of components is not achieved
    // (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
    int num_items = dist_1(generator);
    while(letter_items.size()  < (num_items*3) ){

        // raffles a letter to be included
        std::string item = alp_i[dist_2(generator)];

        // raffles a mounting command to be included
        letter_items.push_back(mountingcom[dist_3(generator)]);
        // adds letter
        letter_items.push_back(item);

        // a raffles a moving command to be included
        letter_items.push_back(movingcom[dist_4(generator)]);

    }

    // build the genetic-string
    GeneticString * gs;
    this->gs = this->build_genetic_string(gs, letter_items);
    this->gs->display_list();

}



/**
 * Genome development: performs replacements in the main genetic-string with production rules for a number of iterations.
 * @param replacement_iterations - number of iterations to perform replacements.
 * @param export_genomes - flag for exporting the main genetic-string to a file.
 */
void Genome::generate_final_string(int  replacement_iterations, int export_genomes, int generation, std::string path){

    // performs replacements for a number of iterations
    for(int i=1; i<=replacement_iterations ;i++) {

        // replacement is made given the grammar
        this->gs->replaces(this->grammar);
    }

    this->gs->display_list();

    if(export_genomes == 1){
        this->exportGenome(path+std::to_string(generation));
    }
}

/**
 * Exports the main-genetic string of a genome to a file.
 */
void Genome::exportGenome(std::string dirpath) {

    std::ofstream genome_file;
    std::string path = "../../experiments/"+dirpath+"/genome" + this->id +".txt";
    genome_file.open(path);

    std::map< std::string, GeneticString >  grammar = std::map< std::string, GeneticString >();

    // writes each production rule of the grammar to a different line in a the file
    for ( auto &it : this->getGrammar()) {

        genome_file << it.first << " "; // writes letter to line

        GeneticString::Node *current;
        current = this->getGrammar()[it.first]->getStart();
        while (current != NULL) {

            genome_file << current->item << " "; // writes all items to line
            current = current->next;
        }
        genome_file << std::endl;
    }
    genome_file.close();
}


/**
* Builds a piece of genetic-string for a genome with the given items.
 * @param _gs - new genetic-string to be populated with the vector of items.
 * @param genetic_string_items - vector of items that will compose teh genetic-string.
 */
GeneticString * Genome::build_genetic_string(GeneticString * _gs, std::vector<std::string> genetic_string_items){

    try {
        _gs->create_list(genetic_string_items);
        return _gs;

    } catch (const std::exception& e) {
        std::cout <<"ERROR building genetic-string: " << e.what() << std::endl;

    }
}


/**
 *  Decodes the genetic-string into a graph of components.
 *  @param LS - Lsystem structure containing the alphabet.
 */
void Genome::decodeGeneticString(LSystem LS,std::map<std::string, double> params){

    try {
        this->dgs = DecodedGeneticString();
        this->dgs.decode(this->gs, LS,  params);

    } catch (const std::exception& e) {
        std::cout <<"ERROR decoding genetic-string: " << e.what() << std::endl;

    }

}


/**
 * Draws a chart from the graph.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param params - list of params read from configuration file.
 * @param path - name of the output directory
 */
void Genome::constructor(int argc, char* argv[], std::map<std::string, double> params, std::string path ) {


    QApplication app(argc, argv);

    this->scene = new QGraphicsScene(); // scene that holds the chart representing the phenotype

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();

    std::vector<QGraphicsRectItem *> items;

    DecodedGeneticString::Vertex * c = NULL;
    c = this->dgs.getRoot();

    // from component on the root, draws all the components in the graph
    this->draw_component("bottom","root",this->scene,items,c,c, params);

    // exports drawn robot into image file
    if (params["export_phenotypes"] == 1) {

        this->scene->clearSelection();                          // Selections would also render to the file
        this->scene->setSceneRect(
                this->scene->itemsBoundingRect());              // Re-shrink the scene to it's bounding contents
        QImage image(this->scene->sceneRect().size().toSize(),
                     QImage::Format_ARGB32);                    // Create the image with the exact size of the shrunk scene
        image.fill(Qt::transparent);                            // Start all pixels transparent
        QPainter painter(&image);
        this->scene->render(&painter);

        QString qstr = QString::fromStdString("../../experiments/"+path+ "/body_" + this->id + "_p1_"+this->id_parent1 + "_p2_" + this->id_parent2+ ".png");
        image.save(qstr);
    }

    // show drawn robot on the screen
    if (params["show_phenotypes"] == 1){
        app.exec();
    }
}


/**
 * Roams the graph, drawing each component of the chart.
 * @param reference - reference of origin-side for the turtle
 * @param direction - direction to which to add the current component, relative to the previous component
 * @param scene - object representing the phenotype
 * @params items - list of components
 * @param c1 - pointer to the root
 * @param c2 - pointer to the current item
 * @param params - list of params read from configuration file.
 */
void Genome::draw_component( std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *> items,  DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2, std::map<std::string, double> params){

    int size = (int) params["size_component"];
    int space = (int) params["spacing"];

    if(c2 != NULL ){ // condition to stop recursive calls

        // draws a new component
        items.push_back(new QGraphicsRectItem());
        // initial default position of a component, which will be repositioned later
        items[items.size()-1]->setRect(0,0,size,size);

        // draws a sign representing the direction (< > ^ v) from the component to its parent
        QGraphicsTextItem * sign = new QGraphicsTextItem;
        scene->addItem(sign);

        // defines colors for the components according to type  (light color has angle in axis x / dark color around z)

        if(c2->item == "C"){
            items[items.size()-1]->setBrush(Qt::yellow); // yellow
        }
        if(c2->item == "B"){
            items[items.size()-1]->setBrush(Qt::blue); // blue
        }
        if(c2->item == "PJ1"){
            items[items.size()-1]->setBrush(QColor(128,246,152));  // light  green
        }
        if(c2->item == "PJ2"){
            items[items.size()-1]->setBrush(QColor(34,122,27));  //  green dark
        }
        if(c2->item == "J1"){
            items[items.size()-1]->setBrush(QColor(182,186,184)); // light grey
        }
        if(c2->item == "J2"){
            items[items.size()-1]->setBrush(QColor(102,105,103));  // dark grey
        }
        if(c2->item == "AJ1"){
            items[items.size()-1]->setBrush(QColor(233,97,118)); // light red
        }
        if(c2->item == "AJ2"){
            items[items.size()-1]->setBrush(Qt::red);  // dark red
        }

        sign->setZValue(1); // sign must be drawn over the component
        scene->addItem(items[items.size()-1]);  // adds new component to the scene


        if(c2 != c1) {  // sets the component (and sign) at the proper position in the drawing

            std::string tsign;

            // aligns component relative to parent's position, given the direction and reference
            // direction is defined according to the mounting command
            // reference is defined according to the turtle path, starting at the bottom

            if(direction == "left") { // if component is supposed to be mounted at the left-side of the parent

                if(reference == "bottom") { // if the back of the turtle is at the bottom-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x - size-space, c2->back->y); // aligns the component relative to parent given direction and reference
                    reference = "rside";  // updates the back of the turtle as right-side, after movement of the turtle
                    tsign = ">";  // sign points to the direction of the parent (right)
                }
                else if(reference == "top") { // if the back of the turtle is at the top-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x + size+space, c2->back->y);
                    reference = "lside"; // updates the back of the turtle as left-side, after movement of the turtle
                    tsign = "<";  // sign points to the direction of the parent (left)
                }
                else if(reference == "lside") { // if the back of the turtle is at the lef- side of the screen
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y-size-space);
                    reference = "bottom";  // updates the back of the turtle as bottom-side, after movement of the turtle
                    tsign = "v";  // sign points to the direction of the parent (down)
                }
                else if(reference == "rside") { // if the back of the turtle is at the right-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+space);
                    reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
                    tsign = "^";  // sign points to the direction of the parent (up)
                }
            }

            if(direction == "right") { // if component is supposed to be mounted at the right-side of the parent

                if(reference == "bottom") { // if the back of the turtle is at the bottom-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x + size+space, c2->back->y);
                    reference = "lside";  // updates the back of the turtle as left-side, after movement of the turtle
                    tsign = "<";  // sign points to the direction of the parent (left)
                }
                else if(reference == "top") { // if the back of the turtle is at the top-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x - size-space, c2->back->y);
                    reference = "rside";  // updates the back of the turtle as right-side, after movement of the turtle
                    tsign = ">";  // sign points to the direction of the parent (right)
                }
                else if(reference == "lside") { // if the back of the turtle is at the left-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+space);
                    reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
                    tsign = "^";  // sign points to the direction of the parent (up)
                }
                else if(reference == "rside") { // if the back of the turtle is at the right-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y-size-space);
                    reference = "bottom";  // updates the back of the turtle as bottom-side, after movement of the turtle
                    tsign = "v";  // sign points to the direction of the parent (down)
                }
            }
            if(direction == "front") { // if component is supposed to be mounted at the front-side of the parent

                if(reference == "bottom") { // if the back of the turtle is at the bottom-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x, c2->back->y-size-space);
                    tsign = "v";   // sign points to the direction of the parent (down)
                }
                else if(reference == "top") { // if the back of the turtle is at the top-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x, c2->back->y+size+space);
                    tsign = "^";   // sign points to the direction of the parent (up)
                }
                else if(reference == "lside") { // if the back of the turtle is at the left-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x+size+space , c2->back->y);
                    tsign = "<";   // sign points to the direction of the parent (left)
                }
                else if(reference == "rside") { // if the back of the turtle is at the right-side of the screen
                    items[items.size() - 1]->setPos(c2->back->x -size-space, c2->back->y);
                    tsign = ">";   // sign points to the direction of the parent (right)
                }
            }
            if(direction == "root-back" ) { // if component is supposed to be mounted at the back-side of the parent

                items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+space);
                reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
                tsign = "^";   // sign points to the direction of the parent (up)
            }

            sign->setPos(items[items.size() - 1]->x()+15 , items[items.size() - 1]->y()+15); // aligns the position of the sign, relative to its component
            sign->setPlainText(QString::fromStdString(tsign)); // draws sign over the component

        }else{
            items[items.size() - 1]->setPos(0, 0); // core-compoemnt is aligned in the 0-0 position
        }

        c2->x = (int) items[items.size()-1]->x(); // saves x coordinate in the graph for the component
        c2->y = (int) items[items.size()-1]->y(); // saves y coordinate in the graph for the component

        QList < QGraphicsItem * > coll;
        coll =  items[items.size()-1]->collidingItems();

        if(coll.size() > 1) { // if the new component is overlapping another component, it is deleted from the graphic

            scene->removeItem(items[items.size()-1]);
            scene->removeItem(sign);

            // removes reference from the parent to the deleted component
            if(direction == "left"){ c2->back->left = NULL; }
            if(direction == "front"){ c2->back->front = NULL; }
            if(direction == "right"){ c2->back->right = NULL; }
            if(direction == "root-back"){ c2->back->back = NULL; }
            c2 = NULL;

            return; // does not keep building from there

        }else{  // saves shortcut for coordinates of the item

            std::pair<int, int> key = std::make_pair(c2->x,c2->y);
            this->list_components[key] = c2->item;
        }

        // recursively calls this function to roam the rest of the graph
        this->draw_component(reference,"left",scene,items,c1,c2->left, params);
        this->draw_component(reference,"front",scene,items,c1,c2->front, params);
        this->draw_component(reference,"right",scene,items,c1,c2->right, params);
        if(c2 == c1){
            this->draw_component(reference,"root-back",scene,items,c1,c2->back, params);
        }
    }


}


/**
*  Creates the genome of an individual in its initial state, as a genetic-string formed only by the axiom.
**/
void Genome::createEmbryo(){

    std::vector<std::string> axiom;
    axiom.push_back("C");

    // initializes the genetic-string with the axiom
    GeneticString * gs = new GeneticString();
    this->setGeneticString(this->build_genetic_string(gs, axiom));

}




/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
* @param argc - command line parameter
* @param argv[] - command line parameter
* @param params - list of params read from configuration file.
* @param LS - Lsystem structure containing the alphabet.
**/

void Genome::developGenomeIndirect(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation, std::string path) {

    // creates main genetic-string for axiom (initial developmental state of the genome)
    this->createEmbryo();

    // enhances the genetic-string according to grammar iteratively
    this->generate_final_string((int) params["replacement_iterations"], (int) params["export_genomes"], generation, path);

    // decodes the final genetic-string into a tree of components
    this->decodeGeneticString(LS, params);

    // generates robot-graphics
    this->constructor(argc, argv, params, path+std::to_string(generation));
}



/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
* @param argc - command line parameter
* @param argv[] - command line parameter
* @param params - list of params read from configuration file.
* @param LS - Lsystem structure containing the alphabet.
**/

//void Genome::developGenomeDirect(int argc, char* argv[], std::map<std::string, double> params, LSystem LS, int generation, std::string path) {
//
//
//    // decodes the final genetic-string into a tree of components
//    this->decodeGeneticString(LS, params);
//
//    // generates robot-graphics
//    this->constructor(argc, argv, params, path+std::to_string(generation));
//}


void Genome::updateFitness(double fitness){

    this->fitness = fitness;
}



std::map< std::string, GeneticString *> Genome::getGrammar(){

    return this->grammar;
}


void Genome::addLetterGrammar(std::string letter, GeneticString * lgs){

    this->grammar.emplace(letter, lgs);
}
