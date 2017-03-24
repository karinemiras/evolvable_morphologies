//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>
#include <random>
#include <string>
#include <fstream>

#include "Genome.h"

unsigned int Genome::getTo() {
    return 3;
}

void Genome::setGeneticString(GeneticString _gs){
    this->gs = _gs;
}

GeneticString Genome::getGeneticString(){
    return this->gs;
}

std::vector<std::string> Genome::getAxiom(){
    return this->axiom;
}

std::string Genome::getId(){
    return this->id;
}


/**
* Generates initial production rules for the alphabet.
*/
void Genome::build_grammar(LSystem LS, int num_initial_comp, double add_backtoparent_prob) {

    std::map< std::string, std::string > alp = LS.getAlphabet();
    std::vector<std::string> alp_i = LS.getAlphabetIndex();
    std::vector<std::string> com = LS.getCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, num_initial_comp); // distribution for the number of components
    std::uniform_int_distribution<int> dist_2(0, alp_i.size()-1); // distribution for letters of the alphabet
    std::uniform_int_distribution<int> dist_3(1, com.size()-1); // distribution for the hatching commands


    for (std::map< std::string, std::string >::const_iterator it = alp.begin(); it != alp.end(); ++it) { // for each letter of the alphabet

        std::string letter = it->first;
        std::cout<<"letter "<<letter<<std::endl;

        std::vector<std::string> letter_items;

        if (letter == "CNNN") { // if it is a core component, forces its inclusion in the rule
            letter_items.push_back(letter);
        }

        while(letter_items.size() < (dist_1(generator)*2) ){  // while raffled number of components is not achieved (times 2 because it must take the commands into account)

            std::string item = alp_i[dist_2(generator)]; // raffles a letter to be included

            if (item != "CNNN") { // prevents core component of being (re)included in the rule
                letter_items.push_back(com[dist_3(generator)]); // raffles a hatching command to be included
                letter_items.push_back(item);

                std::uniform_real_distribution<double> p_btp(0.0, 1.0); // distribution for back-to-parent command
                double p = p_btp(generator);
                std::cout<<" Aaaaaaaaaaaaaakkkkkkkkkiiiiiiiiii " <<p <<"   "<<add_backtoparent_prob;
                if (p < add_backtoparent_prob){
                    letter_items.push_back(com[0]);
                }
            }
        }


        GeneticString lgs;
        lgs = build_genetic_string(lgs, letter_items);

        lgs.display_list();
        this->grammar.emplace(letter, lgs);


    }

}

//    int t;
//    t = 2;
//
//    if (t == 3) {
//
//        // manual production 3
//        std::vector<std::string> letter_c;
//        letter_c.push_back("CNNN");
//        letter_c.push_back("addr");
//        letter_c.push_back("BNNN");
//        GeneticString cgs;
//        cgs = build_genetic_string(cgs, letter_c);
//        cgs.display_list();
//        this->grammar["CNNN"] = cgs;
//
//        std::vector<std::string> letter_b;
//        letter_b.push_back("BNNN");
//        letter_b.push_back("<");
//        letter_b.push_back("<");
//        letter_b.push_back("addl");
//        letter_b.push_back("J1");
//        GeneticString bgs;
//        bgs = build_genetic_string(bgs, letter_b);
//        bgs.display_list();
//        this->grammar["BNNN"] = bgs;
//
//        std::vector<std::string> letter_j;
//        letter_j.push_back("addf");
//        letter_j.push_back("J1");
//        letter_j.push_back("addf");
//        letter_j.push_back("BNNN");
//        letter_j.push_back("<");
//        letter_j.push_back("<");
//        letter_j.push_back("addl");
//        letter_j.push_back("BNNN");
//        letter_j.push_back("addr");
//        letter_j.push_back("BNNN");
//
//
//        GeneticString jgs;
//        jgs = build_genetic_string(jgs, letter_j);
//        jgs.display_list();
//        this->grammar["J1"] = jgs;
//    }
//    if (t == 1) {
//        // manual production 1
//        std::vector<std::string> letter_c;
//        letter_c.push_back("CNNN");
//        letter_c.push_back("addr");
//        letter_c.push_back("BNNN");
//        GeneticString cgs;
//        cgs = build_genetic_string(cgs, letter_c);
//        cgs.display_list();
//        this->grammar["CNNN"] = cgs;
//
//        std::vector<std::string> letter_b;
//        letter_b.push_back("addl");
//        letter_b.push_back("J1");
//        letter_b.push_back("<");
//        letter_b.push_back("addr");
//        letter_b.push_back("J1");
//        GeneticString bgs;
//        bgs = build_genetic_string(bgs, letter_b);
//        bgs.display_list();
//        this->grammar["BNNN"] = bgs;
//
//        std::vector<std::string> letter_j;
//        letter_j.push_back("J1");
//        letter_j.push_back("addf");
//        letter_j.push_back("BNNN");
//        letter_j.push_back("<");
//        letter_j.push_back("<");
//        GeneticString jgs;
//        jgs = build_genetic_string(jgs, letter_j);
//        jgs.display_list();
//        this->grammar["J1"] = jgs;
//    }
//    if (t == 2) {
//        // manual production 2
//        std::vector<std::string> letter_c;
//        letter_c.push_back("CNNN");
//        letter_c.push_back("addr");
//        letter_c.push_back("BNNN");
//        GeneticString cgs;
//        cgs = build_genetic_string(cgs, letter_c);
//        cgs.display_list();
//        this->grammar["CNNN"] = cgs;
//
//        std::vector<std::string> letter_b;
//        letter_b.push_back("addl");
//        letter_b.push_back("J1");
//        letter_b.push_back("<");
//        letter_b.push_back("addr");
//        letter_b.push_back("J1");
//        GeneticString bgs;
//        bgs = build_genetic_string(bgs, letter_b);
//        bgs.display_list();
//        this->grammar["BNNN"] = bgs;
//
//        std::vector<std::string> letter_j;
//        letter_j.push_back("J1");
//        letter_j.push_back("addf");
//        letter_j.push_back("BNNN");
//        letter_j.push_back("addf");
//        letter_j.push_back("AJ");
//        letter_j.push_back("addf");
//        letter_j.push_back("BNNN");
//        letter_j.push_back("<");
//        letter_j.push_back("<");
//        letter_j.push_back("<");
//        letter_j.push_back("<");
//        GeneticString jgs;
//        jgs = build_genetic_string(jgs, letter_j);
//        jgs.display_list();
//        this->grammar["J1"] = jgs;
//
//    }
//
//}


/**
 * Performs replacements with production rules for a number of iterations.
 */
void Genome::generate_final_string(int  replacement_iterations, int export_genomes){

    for(int i=1; i<=replacement_iterations ;i++) {
        this->gs.replaces(this->grammar);
        std::cout << "string iteration " << i << std::endl;
        this->gs.display_list();
    }
    if(export_genomes == 1){
        this->exportGenome();
    }
}

void Genome::exportGenome() {

    std::ofstream genome_file;
    std::string path = "/Users/karinemiras/CLionProjects/lsystem-proto/genome_"+this->id+".txt";
    genome_file.open(path);

    GeneticString::Node *current;
    current = this->gs.getStart();
    while (current != NULL) {
        genome_file << current->item << " ";
        current = current->next;
    }
    genome_file.close();
}

/**
* Builds a piece of genetic-string for a genome with the given items.
 */
GeneticString Genome::build_genetic_string(GeneticString _gs, std::vector<std::string> genetic_string_items){

    try {
        _gs.create_list(genetic_string_items);
        return _gs;

    } catch (const std::exception& e) {
        std::cout <<"ERROR building axiom: " << e.what() << std::endl;

    }
}


/**
 *  Decodes the genetic-string into a graph of components.
 */
void Genome::decodeGeneticString(LSystem LS){

    try {
        this->dgs.decode(this->gs,LS);

    } catch (const std::exception& e) {
        std::cout <<"ERROR decoding genetic-string: " << e.what() << std::endl;

    }

}


/**
 * Draws a chart from the graph.
 */
void Genome::constructor(int argc, char* argv[], int show_phenotypes, int export_phenotypes) {


    QApplication app(argc, argv);

    this->scene = new QGraphicsScene();

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();

    std::vector<QGraphicsRectItem *> items;

    DecodedGeneticString::Vertex * c = NULL;
    c = this->dgs.getRoot();

    this->draw_component("bottom","root",this->scene,items,c,c); // from component on the root, draws all components in the graph

    // exports drawn robot into image file
    if (export_phenotypes == 1) {
        this->scene->clearSelection();                                                  // Selections would also render to the file
        this->scene->setSceneRect(
                this->scene->itemsBoundingRect());                                      // Re-shrink the scene to it's bounding contents
        QImage image(this->scene->sceneRect().size().toSize(),
                     QImage::Format_ARGB32);                                      // Create the image with the exact size of the shrunk scene
        image.fill(Qt::transparent);                                              // Start all pixels transparent
        QPainter painter(&image);
        this->scene->render(&painter);
        QString qstr = QString::fromStdString("/Users/karinemiras/CLionProjects/lsystem-proto/" + this->id + ".png");
        image.save(qstr);
    }

    // show drawn robot on the screen
    if (show_phenotypes == 1){
        app.exec();
    }
}


/**
 * Roams the graph, drawing each component of the chart.
 */
void Genome::draw_component( std::string reference, std::string direction, QGraphicsScene * scene, std::vector<QGraphicsRectItem *> items,  DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2){

    int size = 40;

    if(c2 != NULL){ // condition to stop recursive calls

        items.push_back(new QGraphicsRectItem()); // draws a new component
        items[items.size()-1]->setRect(0,0,size,size);

        QGraphicsTextItem * sign = new QGraphicsTextItem; // draws a sign (direction) from the component to its parent
        scene->addItem(sign);

        // defines colors for the components according to type

        if(c2->item == "CNNN"){
            items[items.size()-1]->setBrush(Qt::yellow);
        }
        if(c2->item == "BNNN"){
            items[items.size()-1]->setBrush(Qt::blue);
        }
        if(c2->item == "PJ"){
            items[items.size()-1]->setBrush(Qt::green);
        }
        if(c2->item == "J1"){
            items[items.size()-1]->setBrush(Qt::white);
        }
        if(c2->item == "AJ"){
            items[items.size()-1]->setBrush(Qt::red);
        }

        sign->setZValue(1); // sign must be drawn over the component
        scene->addItem(items[items.size()-1]);  // adds new component to the scene


        if(c2 != c1) {  // sets the component (and sign) in the proper position in the drawing

            std::string tsign;

            if(direction == "left") { // direction is accprding to the command

                if(reference == "bottom") {   // reference is according to the turtle path, starting in the bottom
                    items[items.size() - 1]->setPos(c2->back->x - size-1, c2->back->y);
                    reference = "rside";
                    tsign = ">";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->x + size+1, c2->back->y);
                    reference = "lside";
                    tsign = "<";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y-size+-1);
                    reference = "bottom";
                    tsign = "v";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+1);
                    reference = "top";
                    tsign = "^";
                }
            }
            if(direction == "right") {

                if(reference == "bottom") {
                    items[items.size() - 1]->setPos(c2->back->x + size+1, c2->back->y);
                    reference = "lside";
                    tsign = "<";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->x - size-1, c2->back->y);
                    reference = "rside";
                    tsign = ">";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+1);
                    reference = "top";
                    tsign = "^";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->x , c2->back->y-size-1);
                    reference = "bottom";
                    tsign = "v";
                }
            }
            if(direction == "front") {

                if(reference == "bottom") {
                    items[items.size() - 1]->setPos(c2->back->x, c2->back->y-size-1);
                    tsign = "v";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->x, c2->back->y+size+1);
                    tsign = "^";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->x+size+1 , c2->back->y);
                    tsign = "<";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->x -size-1, c2->back->y);
                    tsign = ">";
                }
            }
            if(direction == "root-back" ) {

                items[items.size() - 1]->setPos(c2->back->x , c2->back->y+size+1);
                reference = "top";
                tsign = "^";
            }

            sign->setPos(items[items.size() - 1]->x()+15 , items[items.size() - 1]->y()+15);
            sign->setPlainText(QString::fromStdString(tsign));

        }else{
                items[items.size() - 1]->setPos(250, 250);
        }

        c2->x = items[items.size()-1]->x(); // saves x coordinate for the component in the graph
        c2->y = items[items.size()-1]->y(); // saves y coordinate for the component in the graph

        QList < QGraphicsItem * > coll;
        coll =  items[items.size()-1]->collidingItems();

        if(coll.size() > 1) { // if the new component is overlapping another component, it is deleted from the graphical
            scene->removeItem(items[items.size()-1]);
            scene->removeItem(sign);

            // removes reference from the parent to the deleted component
            if(direction == "left"){ c2->back->left = NULL; }
            if(direction == "front"){ c2->back->front = NULL; }
            if(direction == "right"){ c2->back->right = NULL; }
            if(direction == "root-back"){ c2->back->back = NULL; }

            return; // does not keep building from there
        }

        // recursively calls this function to roam the rest of the graph
        this->draw_component(reference,"left",scene,items,c1,c2->left);
        this->draw_component(reference,"front",scene,items,c1,c2->front);
        this->draw_component(reference,"right",scene,items,c1,c2->right);
        if(c2 == c1){
            this->draw_component(reference,"root-back",scene,items,c1,c2->back);
        }
    }


}


/**
*  Creates the genome of an individual in its initial state, as a genetic-string formed only by the axiom.
**/
void Genome::createEmbryo(){

    std::vector<std::string> axiom;
    axiom.push_back("CNNN");

    // initializes the genetic-string with the axiom
    this->setGeneticString(this->build_genetic_string(this->getGeneticString(), axiom));

    std::cout << " >> building axiom ..." << std::endl;
    this->getGeneticString().display_list();

}




/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
**/

void Genome::developGenome(int argc, char* argv[], std::map<std::string, double> params, LSystem LS) {

    // creates genetic-strings for production initial rules with the grammar
    std::cout << " >> building grammar ..." << std::endl;
    this->build_grammar(LS, params["num_initial_comp"], params["add_backtoparent_prob"]);

    // enhances the genetic-string according to grammar iteratively
    std::cout << " >> iterating replacements ..." << std::endl;
    this->generate_final_string(params["replacement_iterations"], params["export_genomes"]);

    // decodes the final genetic-string into a tree of components
    std::cout << " >> decoding ... " << std::endl;
    this->decodeGeneticString(LS);

    // generates robot-graphics
    std::cout << " >> constructing ... " << std::endl;
    this->constructor(argc, argv, params["show_phenotypes"], params["export_phenotypes"]);
}



/**
*  Measures several metrics about the phenotype of an individual.
**/
void Genome::measurePhenotype(){

    DecodedGeneticString::Vertex * c = NULL;
    c = this->dgs.getRoot();

    this->measureComponent(c,c);

    std::ofstream measures_file;
    std::string path = "/Users/karinemiras/CLionProjects/lsystem-proto/measures_"+this->id+".txt";
    measures_file.open(path);

    for (std::map< std::string, double >::const_iterator it = this->measures.begin(); it != this->measures.end(); ++it) {
        measures_file << it->first << " : " << it->second << std::endl;
    }
    measures_file.close();
}

void Genome::measureComponent(DecodedGeneticString::Vertex * c1, DecodedGeneticString::Vertex * c2){

    if(c2 != NULL){ // condition to stop recursive calls


        if (c2->item == "BNNN") { this->measures["total_bricks"]++; } // counts for each brick
        if (c2->item == "J1") { this->measures["total_fixed_joints"]++; } // counts for each fixed joint
        if (c2->item == "PJ") { this->measures["total_passive_joints"]++; } // counts for each passive joint
        if (c2->item == "AJ") { this->measures["total_active_joints"]++; } // counts for each active joint

        int connected_sides = 0;
        if (c1 != c2) {
            connected_sides++; // counts for parent connectivity
        } else {
            if (c2->back != NULL) { connected_sides++; }
        } // if it is root, and theres back-side connectivity
        if (c2->left != NULL) { connected_sides++; } //  if theres left-side connectivity
        if (c2->right != NULL) { connected_sides++; } // if theres right-side connectivity
        if (c2->front != NULL) { connected_sides++; } //  if theres front-side connectivity
        if (connected_sides == 1) { this->measures["connectivity1"]++; } // counts for only one side is connected
        if (connected_sides == 2) {
            this->measures["connectivity2"]++; // counts for only two sides are connected

            if(c2->item == "J1" or c2->item == "PJ" or c2->item == "AJ") { this->measures["effective_joints"]++; } // counts for joints connected by both sides
        }
        if (connected_sides == 3) { this->measures["connectivity3"]++; } // counts for only three sides are connected
        if (connected_sides == 4) { this->measures["connectivity4"]++; } // counts for all four sides are connected

        // recursively calls this function to roam the rest of the graphical because it was in violation
        this->measureComponent(c1, c2->left);
        this->measureComponent(c1, c2->front);
        this->measureComponent(c1, c2->right);
        if (c2 == c1) {
            this->measureComponent(c1, c2->back);
        }

    }

}

void Genome::initalizeMeasures(){

    this->measures["total_bricks"] = 0; // measure: total amount of brick-components forming the robot
    this->measures["total_fixed_joints"] = 0; // measure: total amount of fixed-joint-components forming the robot
    this->measures["total_passive_joints"] = 0; // measure: total amount of passive-joint-components forming the robot
    this->measures["total_active_joints"] = 0; // measure: total amount of active-joint-components forming the robot
    this->measures["connectivity1"] = 0;  // measure: total of components with one side connected to another component
    this->measures["connectivity2"] = 0;  // measure: total of components with two sides connected to another component
    this->measures["connectivity3"] = 0;  // measure: total of components with three sides connected to another component
    this->measures["connectivity4"] = 0;  // measure: total of components with four sides connected to another component
    this->measures["effective_joints"] = 0;  // measure: total of joints connected by both sides
}

std::map< std::string, double > Genome::getMeasures() {
    return this->measures;
}


