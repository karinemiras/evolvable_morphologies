//
// Created by Karine Miras on 07/03/2017.
//

#include <iostream>
#include <random>
#include <string>

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



/**
* Generates initial production rules for the alphabet.
*/
void Genome::build_grammar(LSystem LS) {

    std::map< std::string, std::string > alp = LS.getAlphabet();
    std::vector<std::string>  alp_i = LS.getAlphabetIndex();
    std::vector<std::string>  com = LS.getCommands();

    std::random_device rd;
    std::default_random_engine generator(rd());

    std::uniform_int_distribution<int> dist_1(1, 3); // distribution for the number of components
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
                if (p_btp(generator) < 0.2){
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
void Genome::generate_final_string(){

    int iterations = 3;

    for(int i=0;i<iterations;i++) {
        this->gs.replaces(this->grammar);
        std::cout << "string iteration " << i << std::endl;
        this->gs.display_list();
    }

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
void Genome::constructor(int argc, char* argv[]) {


    QApplication app(argc, argv);

    QGraphicsScene * scene = new QGraphicsScene();

    QGraphicsView * view = new QGraphicsView(scene);
    view->show();

    std::vector<QGraphicsRectItem *> items;

    DecodedGeneticString::Vertex * c = NULL;
    c = this->dgs.getRoot();
    std::cout<<" will draw "<<std::endl;
    Genome::draw_component("bottom","root",scene,items,c,c); // from component on the root, draws all components in the graph

    // exports drawn robot into image file
    scene->clearSelection();                                                  // Selections would also render to the file
    scene->setSceneRect(scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);                                              // Start all pixels transparent
    QPainter painter(&image);
    scene->render(&painter);
    QString qstr = QString::fromStdString("/Users/karinemiras/CLionProjects/lsystem-proto/"+this->id+".png");
    image.save(qstr);

    //app.exec();
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

        c2->comp = items[items.size()-1]; // saves reference for the component inside the graph-node

        sign->setZValue(1); // sign must be drawn over the component
        scene->addItem(items[items.size()-1]);  // adds new component to the scene


        if(c2 != c1) {  // sets the component (and sign) in the proper position in the drawing

            std::string tsign;

            if(direction == "left") {

                if(reference == "bottom") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() - size-1, c2->back->comp->y());
                    reference = "rside";
                    tsign = ">";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() + size+1, c2->back->comp->y());
                    reference = "lside";
                    tsign = "<";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() , c2->back->comp->y()-size+-1);
                    reference = "bottom";
                    tsign = "v";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() , c2->back->comp->y()+size+1);
                    reference = "top";
                    tsign = "^";
                }
            }
            if(direction == "right") {

                if(reference == "bottom") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() + size+1, c2->back->comp->y());
                    reference = "lside";
                    tsign = "<";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() - size-1, c2->back->comp->y());
                    reference = "rside";
                    tsign = ">";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() , c2->back->comp->y()+size+1);
                    reference = "top";
                    tsign = "^";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() , c2->back->comp->y()-size-1);
                    reference = "bottom";
                    tsign = "v";
                }
            }
            if(direction == "front") {

                if(reference == "bottom") {
                    items[items.size() - 1]->setPos(c2->back->comp->x(), c2->back->comp->y()-size-1);
                    tsign = "v";
                }
                else if(reference == "top") {
                    items[items.size() - 1]->setPos(c2->back->comp->x(), c2->back->comp->y()+size+1);
                    tsign = "^";
                }
                else if(reference == "lside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x()+size+1 , c2->back->comp->y());
                    tsign = "<";
                }
                else if(reference == "rside") {
                    items[items.size() - 1]->setPos(c2->back->comp->x() -size-1, c2->back->comp->y());
                    tsign = ">";
                }
            }
            if(direction == "root-back" ) {

                items[items.size() - 1]->setPos(c2->back->comp->x() , c2->back->comp->y()+size+1);
                reference = "top";
                tsign = "^";
            }

            sign->setPos(items[items.size() - 1]->x()+15 , items[items.size() - 1]->y()+15);
            sign->setPlainText(QString::fromStdString(tsign));

        }else{
                items[items.size() - 1]->setPos(250, 250);
        }



        QList < QGraphicsItem * > coll;
        coll =  items[items.size()-1]->collidingItems();

        if(coll.size() > 1) { // if the new component is overlapping another component, it is removed from the graph

            scene->removeItem(items[items.size()-1]);
            scene->removeItem(sign);
           // c2->comp = NULL;
        }

        // recursively calls this function to roam the rest of the graph

        Genome::draw_component(reference,"left",scene,items,c1,c2->left);
        Genome::draw_component(reference,"front",scene,items,c1,c2->front);
        Genome::draw_component(reference,"right",scene,items,c1,c2->right);
        if(c2 == c1){
            Genome::draw_component(reference,"root-back",scene,items,c1,c2->back);
        }
    }


}












