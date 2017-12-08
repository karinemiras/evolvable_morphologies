//
// Created by Karine Miras on 07/03/2017.
//


#include <fstream>
#include <iostream>
#include <random>

#include "Aux.h"
#include "Genome.h"


unsigned int Genome::getTo()
{
  //return 4; //test of testing
}

void Genome::setGeneticString(GeneticString  _gs)
{
  this->gs = _gs;
}

GeneticString  Genome::getGeneticString()
{
  return this->gs;
}

std::string Genome::getId()
{
  return this->id;
}

std::string Genome::getId_parent1()
{
  return this->id_parent1;
}

std::string Genome::getId_parent2()
{
  return this->id_parent2;
}

double Genome::getFit_parent1()
{
  return this->fit_parent1;
}

double Genome::getFit_parent2()
{
  return this->fit_parent2;
}

DecodedGeneticString Genome::getDgs()
{
  return this->dgs;
}

std::map< std::string, double > Genome::getMeasures()
{
  return this->measures;
};

std::map< std::pair< int, int >, std::string > Genome::getList_components()
{
  return this->list_components;
};

void Genome::updateMeasure(
    std::string key,
    double value)
{
  this->measures[key] = value;
}

void Genome::removeMeasure(std::string key)
{

  std::map< std::string, double >::iterator it;

  it = this->measures.find(key);
  this->measures.erase(it);

}

QGraphicsScene * Genome::getScene(){
  return this->scene;
}


void Genome::setGrammar(
    std::map< std::string, GeneticString  > grammar)
{
  this->grammar = grammar;
}

double Genome::getFitness()
{
  return this->fitness;
}

/**
* Generates initial production rules for the alphabet.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void Genome::build_grammar(
    LSystem LS,
    std::map< std::string, double > params)
{

  std::random_device rd;
  std::default_random_engine generator(rd());

  // distribution for the number of components
  std::uniform_int_distribution< int > dist_1(
      1,
      (int) params["num_initial_comp"]);
  // distribution for letters of the alphabet
  std::uniform_int_distribution< int > dist_2(
      0,
      (int) LS.getAlphabetIndex().size() -
      1);
  // distribution for the mounting commands
  std::uniform_int_distribution< int > dist_3(
      0,
      (int) LS.getMountingCommands().size() -
      1);
  // distribution for the moving commands
  std::uniform_int_distribution< int > dist_4(
      0,
      (int) LS.getMovingCommands().size() -
      1);
  // distribution for the brain moving commands
  std::uniform_int_distribution< int > dist_5(
      0,
      (int) LS.getBrainMoveCommands().size() -
      1);
  // distribution for the brain change commands
  std::uniform_int_distribution< int > dist_6(
      0,
      (int) LS.getBrainChangeCommands().size() -
      1);

  // for each letter of the alphabet
  for (const auto &letterPair : LS.getAlphabet())
  {
    auto letter = letterPair.first;

    std::vector< std::string > letter_items;

    if (letter == "C")
    { // if it is a core component, forces its inclusion in the rule
      letter_items.push_back(letter);
    }

    // while a raffled number of components is not achieved
    // (times group size (6):
    // brainmove, mountcom, letter, movecom, brainmove, brainchange
    int groups = dist_1(generator);
    while (letter_items.size() < (groups * 6))
    {
      // raffles a letter to be included
      auto item = LS.getAlphabetIndex()[
          dist_2(generator)];

      // prevents core component of being (re)included in the rule
      if (item != "C")
      {
        // if its a joint, adds neuron info
        if(item.substr(0,1) == "A" or item.substr(0,1) == "S"){
          item = LS.buildBrainCommand(item);
        }

        // raffles a brain move command to be included
        auto braincommand = LS.buildBrainCommand(LS.getBrainMoveCommands()[dist_5(generator)]);
        letter_items.push_back(braincommand);

        // raffles a mounting command to be included
        letter_items.push_back(LS.getMountingCommands()[dist_3(generator)]);

        // raffles a letter to be included
        letter_items.push_back(item);

        // raffles a moving command to be included
        letter_items.push_back(LS.getMovingCommands()[dist_4(generator)]);

        // raffles another brain move command to be included
        braincommand = LS.buildBrainCommand(LS.getBrainMoveCommands()[dist_5(generator)]);
        letter_items.push_back(braincommand);

        // raffles a brain change command to be included
        braincommand = LS.buildBrainCommand(LS.getBrainChangeCommands()[dist_6(generator)]);
        letter_items.push_back(braincommand);

      }
    }

    // build a genetic-string with the the production rule for the letter
    GeneticString lgs =  GeneticString();

    this->build_genetic_string(
        lgs,
        letter_items);

    // adds letter and its production rule (made a genetic-string) to the grammar of the genome
    this->grammar.emplace(
        letter,
        lgs);
  }
}


/**
* Generates initial genomes for the direct encoding.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
*/
void Genome::build_genome_direct(
    LSystem LS,
    std::map< std::string, double > params)
{

  auto alp_i = LS.getAlphabetIndex();
  auto mountingcom = LS.getMountingCommands();
  auto movingcom = LS.getMovingCommands();

  std::random_device rd;
  std::default_random_engine generator(rd());

  // distribution for the number of components
  std::uniform_int_distribution< int > dist_1(
      1,
      (int) params["max_comps"]);
  // distribution for letters of the alphabet, excluding 0 to prevent core-component of being (re)included in the rule
  std::uniform_int_distribution< int > dist_2(
      1,
      (int) alp_i.size() - 1);
  // distribution for the mounting commands
  std::uniform_int_distribution< int > dist_3(
      0,
      (int) mountingcom.size() - 1);
  // distribution for the moving commands
  std::uniform_int_distribution< int > dist_4(
      0,
      (int) movingcom.size() - 1);

  std::vector< std::string > letter_items;

  // forces core-component inclusion in the rule as first item of the genetic string
  letter_items.push_back("C");

  // while a raffled number of components is not achieved
  // (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
  int num_items = dist_1(generator);
  while (letter_items.size() < (num_items * 3))
  {

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
  GeneticString  gs = GeneticString();
  this->build_genetic_string(
      gs,
      letter_items);
  this->gs.display_list();

}


/**
 * Genome development: performs replacements in the main genetic-string with production rules for a number of iterations.
 * @param replacement_iterations - number of iterations to perform replacements.
 * @param export_genomes - flag for exporting the main genetic-string to a file.
 */
void Genome::generate_final_string(
    int replacement_iterations,
    int export_genomes,
    int generation,
    std::string path)
{

  // performs replacements for a number of iterations
  for (int i = 1; i <= replacement_iterations; i++)
  {
    // replacement is made given the grammar
    this->gs.replaces(this->grammar);
  }

  this->gs.display_list();

  if (export_genomes == 1)
  {
    this->exportGenome(path + std::to_string(generation));
  }
}

/**
 * Exports the main-genetic string of a genome to a file.
 */
void Genome::exportGenome(std::string dirpath)
{

  std::ofstream genome_file;
  std::string path =
      "../../experiments/" + dirpath + "/genome" + this->id + ".txt";
  genome_file.open(path);

  std::map< std::string, GeneticString > grammar = std::map< std::string, GeneticString >();

  // writes each production rule of the grammar to a different line in a the file
  for (auto &it : this->getGrammar())
  {

    genome_file << it.first << " "; // writes letter to line

    GeneticString::Node *current;
    current = this->getGrammar()[it.first].getStart();
    while (current != NULL)
    {

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
GeneticString  Genome::build_genetic_string(
    GeneticString  &_gs,
    std::vector< std::string > genetic_string_items)
{
  try
  {
    _gs.create_list(genetic_string_items);
    return _gs;

  } catch (const std::exception &e)
  {
    std::cout << "ERROR building genetic-string: " << e.what() << std::endl;
  }
}


/**
 *  Decodes the genetic-string into a graph of components.
 *  @param LS - Lsystem structure containing the alphabet.
 */
void Genome::decodeGeneticString(
    LSystem LS,
    std::map< std::string, double > params,
    std::string path)
{

  try
  {
    this->dgs = DecodedGeneticString();
    this->dgs.decode(
        this->gs,
        LS,
        params,
        path);

  } catch (const std::exception &e)
  {
    std::cout << "ERROR decoding genetic-string: " << e.what() << std::endl;

  }

}


/**
 * Draws a chart from the graph.
 * @param argc - command line parameter
 * @param argv[] - command line parameter
 * @param params - list of params read from configuration file.
 * @param path - name of the output directory
 */
void Genome::constructor(
    int argc,
    char *argv[],
    std::map< std::string, double > params,
    std::string path)
{


  QApplication app(
      argc,
      argv);

  this->scene = new QGraphicsScene(); // scene that holds the chart representing the phenotype

  QGraphicsView *view = new QGraphicsView(scene);
  view->show();

  std::vector< QGraphicsRectItem * > items;

  DecodedGeneticString::Vertex *c = NULL;
  c = this->dgs.getRoot();


  // from component on the root, draws all the components in the body graph
  this->draw_component(
      "",
      0,
      path,
      "bottom",
      "root",
      this->scene,
      items,
      c,
      c,
      params);

  this->convertYamlBrain(path);

  // exports drawn robot into image file
  if (params["export_phenotypes"] == 1)
  {
    this->scene->clearSelection();                          // Selections would also render to the file
    this->scene->setSceneRect(
        this->scene->itemsBoundingRect());              // Re-shrink the scene to it's bounding contents
    QImage image(
        this->scene->sceneRect().size().toSize(),
        QImage::Format_ARGB32);                    // Create the image with the exact size of the shrunk scene
    image.fill(Qt::transparent);                            // Start all pixels transparent
    QPainter painter(&image);
    this->scene->render(&painter);

    QString qstr = QString::fromStdString(
        "../../experiments/" +
        path + "/body_" +
        this->id + "_p1_" +
        this->id_parent1 + "_p2_" +
        this->id_parent2 + ".png");
    image.save(qstr);

    // draw brain graph
    std::string auxcom =
        "dot -Tpng ../../experiments/" + path + "/tempbrain"
            ".dot "
            "-o ../../experiments/" + path + "/brain_" + this->id +
        ".png";
    std::system(auxcom.c_str());
  }

  // show drawn robot on the screen
  if (params["show_phenotypes"] == 1)
  {
    app.exec();
  }
}


/**
 * Roams the graph, drawing each component of the chart.
 * @param parent_convertion - letter of the parent component
 * @param convertion_level - level of hierarchy of body components for yaml
 * @param _directoryPath - path to save yaml files
 * @param reference - reference of origin-side for the turtle
 * @param direction - direction to which to add the current component, relative to the previous component
 * @param scene - object representing the phenotype
 * @params items - list of components
 * @param c1 - pointer to the root
 * @param c2 - pointer to the current item
 * @param params - list of params read from configuration file.
 */
void Genome::draw_component(
    std::string parent_convertion,
    int convertion_level,
    std::string _directoryPath,
    std::string reference,
    std::string direction,
    QGraphicsScene *scene,
    std::vector< QGraphicsRectItem * > items,
    DecodedGeneticString::Vertex *c1,
    DecodedGeneticString::Vertex *c2,
    std::map< std::string, double > params)
{

  int size = (int) params["size_component"];
  int space = (int) params["spacing"];
  std::string letter_convertion = "";

  if (c2 != NULL)
  { // condition to stop recursive calls

    // draws a new component
    items.push_back(new QGraphicsRectItem());
    // initial default position of a component, which will be repositioned later
    items[items.size() - 1]->setRect(
        0,
        0,
        size,
        size);

    // draws a sign representing the direction (< > ^ v) from the component to its parent
    QGraphicsTextItem *sign = new QGraphicsTextItem;
    scene->addItem(sign);

    // defines colors for the components according to type  (light color has angle in axis x / dark color around z)

    if (c2->item == "C")
    {
      items[items.size() - 1]->setBrush(Qt::yellow); // yellow
    }
    if (c2->item == "B")
    {
      items[items.size() - 1]->setBrush(Qt::blue); // blue
    }
    if (c2->item == "PJ1")
    {
      items[items.size() - 1]->setBrush(
          QColor(
              128,
              246,
              152));  // light  green
    }
    if (c2->item == "PJ2")
    {
      items[items.size() - 1]->setBrush(
          QColor(
              34,
              122,
              27));  //  green dark
    }
    if (c2->item == "J1")
    {
      items[items.size() - 1]->setBrush(
          QColor(
              182,
              186,
              184)); // light grey
    }
    if (c2->item == "J2")
    {
      items[items.size() - 1]->setBrush(
          QColor(
              102,
              105,
              103));  // dark grey
    }
    if (c2->item == "AJ1")
    {
      items[items.size() - 1]->setBrush(
          QColor(
              233,
              97,
              118)); // light red
    }
    if (c2->item == "AJ2")
    {
      items[items.size() - 1]->setBrush(Qt::red);  // dark red
    }

    sign->setZValue(1); // sign must be drawn over the component

    scene->addItem(
        items[items.size() -
              1]);  // adds new component to the scene

    std::string tsign;

    if (c2 != c1)
    {  // sets the component (and sign) at the proper position in the drawing


      // aligns component relative to parent's position, given the direction and reference
      // direction is defined according to the mounting command
      // reference is defined according to the turtle path, starting at the bottom

      if (direction == "left")
      { // if component is supposed to be mounted at the left-side of the parent

        if (reference == "bottom")
        { // if the back of the turtle is at the bottom-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x - size - space,
              c2->back->y); // aligns the component relative to parent given direction and reference
          reference = "rside";  // updates the back of the turtle as right-side, after movement of the turtle
          tsign = ">";  // sign points to the direction of the parent (right)
        }
        else if (reference == "top")
        { // if the back of the turtle is at the top-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x + size + space,
              c2->back->y);
          reference = "lside"; // updates the back of the turtle as left-side, after movement of the turtle
          tsign = "<";  // sign points to the direction of the parent (left)
        }
        else if (reference == "lside")
        { // if the back of the turtle is at the lef- side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y - size - space);
          reference = "bottom";  // updates the back of the turtle as bottom-side, after movement of the turtle
          tsign = "v";  // sign points to the direction of the parent (down)
        }
        else if (reference == "rside")
        { // if the back of the turtle is at the right-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y + size + space);
          reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
          tsign = "^";  // sign points to the direction of the parent (up)
        }
      }

      if (direction == "right")
      { // if component is supposed to be mounted at the right-side of the parent

        if (reference == "bottom")
        { // if the back of the turtle is at the bottom-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x + size + space,
              c2->back->y);
          reference = "lside";  // updates the back of the turtle as left-side, after movement of the turtle
          tsign = "<";  // sign points to the direction of the parent (left)
        }
        else if (reference == "top")
        { // if the back of the turtle is at the top-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x - size - space,
              c2->back->y);
          reference = "rside";  // updates the back of the turtle as right-side, after movement of the turtle
          tsign = ">";  // sign points to the direction of the parent (right)
        }
        else if (reference == "lside")
        { // if the back of the turtle is at the left-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y + size + space);
          reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
          tsign = "^";  // sign points to the direction of the parent (up)
        }
        else if (reference == "rside")
        { // if the back of the turtle is at the right-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y - size - space);
          reference = "bottom";  // updates the back of the turtle as bottom-side, after movement of the turtle
          tsign = "v";  // sign points to the direction of the parent (down)
        }
      }
      if (direction == "front")
      { // if component is supposed to be mounted at the front-side of the parent

        if (reference == "bottom")
        { // if the back of the turtle is at the bottom-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y - size - space);
          tsign = "v";   // sign points to the direction of the parent (down)
        }
        else if (reference == "top")
        { // if the back of the turtle is at the top-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x,
              c2->back->y + size + space);
          tsign = "^";   // sign points to the direction of the parent (up)
        }
        else if (reference == "lside")
        { // if the back of the turtle is at the left-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x + size + space,
              c2->back->y);
          tsign = "<";   // sign points to the direction of the parent (left)
        }
        else if (reference == "rside")
        { // if the back of the turtle is at the right-side of the screen
          items[items.size() - 1]->setPos(
              c2->back->x - size - space,
              c2->back->y);
          tsign = ">";   // sign points to the direction of the parent (right)
        }
      }
      if (direction == "root-back")
      { // if component is supposed to be mounted at the back-side of the parent

        items[items.size() - 1]->setPos(
            c2->back->x,
            c2->back->y + size + space);
        reference = "top";  // updates the back of the turtle as top-side, after movement of the turtle
        tsign = "^";   // sign points to the direction of the parent (up)
      }


    }
    else
    {
      items[items.size() - 1]->setPos(
          0,
          0); // core-compoemnt is aligned in the 0-0 position
    }

    // add info to the components
    tsign = std::to_string(c2->id) + " " + tsign + "\n";
    tsign = tsign
            + c2->sensor_left.substr(1, 1)
            + c2->sensor_front.substr(1, 1)
            + c2->sensor_right.substr(1, 1);
    if (c2 == c1)
    {
      tsign = tsign + c2->sensor_back.substr(1, 1);
    }

    sign->setPlainText(QString::fromStdString(tsign)); //draws sign over the component

    // aligns the position of the sign, relative to its component
    sign->setPos(
        items[items.size() - 1]->x() + 0.001,
        items[items.size() - 1]->y() + 7);


    c2->x = (int) items[items.size() -
                        1]->x(); // saves x coordinate in the graph for the component
    c2->y = (int) items[items.size() -
                        1]->y(); // saves y coordinate in the graph for the component

    QList< QGraphicsItem * > coll;
    coll = items[items.size() - 1]->collidingItems();

    // if the new component is overlapping another component
    if (coll.size() > 1)
    {

      this->valid = 0;

      scene->removeItem(items[items.size() - 1]);
      scene->removeItem(sign);

      // removes reference from the parent to the deleted component
      if (direction == "left")
      { c2->back->left = NULL; }
      if (direction == "front")
      { c2->back->front = NULL; }
      if (direction == "right")
      { c2->back->right = NULL; }
      if (direction == "root-back")
      { c2->back->back = NULL; }
      c2 = NULL;

      return; // does not keep building from there

    }
    else
    {  // saves shortcut for coordinates of the item


      std::pair< int, int > key = std::make_pair(
          c2->x,
          c2->y);
      this->list_components[key] = c2->item;


      // writes the module into yaml file
      this->convertYamlBody(
          parent_convertion,
          _directoryPath,
          convertion_level,
          direction,
          c2,
          "");
      // writes the module's sonsors into yaml file
      if (c2->sensor_left != "Sn")
      {
        this->convertYamlBody(
            c2->item,
            _directoryPath,
            convertion_level + 1,
            "left",
            c2,
            c2->sensor_left);
      }
      if (c2->sensor_front != "Sn")
      {
        this->convertYamlBody(
            c2->item,
            _directoryPath,
            convertion_level + 1,
            "front",
            c2,
            c2->sensor_front);
      }
      if (c2->sensor_right != "Sn")
      {
        this->convertYamlBody(
            c2->item,
            _directoryPath,
            convertion_level + 1,
            "right",
            c2,
            c2->sensor_right);
      }
      if (c2->sensor_back != "Sn")
      {
        this->convertYamlBody(
            c2->item,
            _directoryPath,
            convertion_level + 1,
            "root-back",
            c2,
            c2->sensor_back);
      }

    }


    // recursively calls this function to roam the rest of the graph
    this->draw_component(
        c2->item,
        convertion_level + 1,
        _directoryPath,
        reference,
        "left",
        scene,
        items,
        c1,
        c2->left,
        params);

    this->draw_component(
        c2->item,
        convertion_level + 1,
        _directoryPath,
        reference,
        "front",
        scene,
        items,
        c1,
        c2->front,
        params);

    this->draw_component(
        c2->item,
        convertion_level + 1,
        _directoryPath,
        reference,
        "right",
        scene,
        items,
        c1,
        c2->right,
        params);

    if (c2 == c1)
    {
      this->draw_component(
          c2->item,
          convertion_level + 1,
          _directoryPath,
          reference,
          "root-back",
          scene,
          items,
          c1,
          c2->back,
          params);
    }
  }
}


void Genome::convertYamlBrain(std::string _directoryPath)
{
  std::ofstream robot_file;
  auto path =
      "../../experiments/" + _directoryPath
      + "/robot_" + this->getId() + ".yaml";
  robot_file.open(
      path,
      std::ofstream::app);

  robot_file << "brain:" << std::endl;
  robot_file << "  neurons:" << std::endl;


  for (const auto &node : this->dgs.getBrain_nodes())
  {
    robot_file << "    node" << node.first << ":" << std::endl;
    robot_file << "      id: node" << node.first << std::endl;
    robot_file << "      layer: " << node.second->layer << std::endl;
    robot_file << "      part_id: module" << node.second->id_comp;
    // if its a sensor in a direction
    if (node.second->direction != "")
    {
      robot_file << "sensor-" << node.second->direction;
    }
    robot_file << std::endl;
    robot_file << "      type: " << node.second->function << std::endl;
  }

  robot_file << "  connections:" << std::endl;

  for (const auto &edge : this->dgs.getBrain_edges())
  {
    auto origin = edge.first.first;
    auto destination = edge.first.second;
    auto weight = edge.second;

    robot_file << "  - dst: node" << destination << std::endl;
    robot_file << "    src: node" << origin << std::endl;
    robot_file << "    weight: " << weight << std::endl;
  }

  // adds parameters
  robot_file << "  params:" << std::endl;

  for (const auto &node : this->dgs.getBrain_nodes())
  {
    if (node.second->function == "Oscillator" )
    {
      robot_file << "    node" << node.first << ":" << std::endl;
      robot_file << "      amplitude: " << node.second->amplitude << std::endl;
      robot_file << "      period: " << node.second->period << std::endl;
      robot_file << "      phase_offset: " << node.second->phase_offset << std::endl;
    }else
    {
      if(node.second->layer != "input")
      {
        robot_file << "    node" << node.first << ":" << std::endl;
        robot_file << "      bias: " << node.second->bias << std::endl;
        robot_file << "      gain: 1"<< std::endl;
      }
    }
  }
  robot_file.close();
}


/*
 * Converts a developed genome into a yaml file.
 * @param parent_convertion - parent component of the current component
 * @param _directoryPath - path to save the yaml file
 * @param convertion_level - hierarchy level in the tree of components
 * @param direction - direction on which current component will be mounted
 * @param c2 - current node (component) pointer
 * */
void Genome::convertYamlBody(
    std::string parent_convertion,
    std::string _directoryPath,
    int convertion_level,
    std::string direction,
    DecodedGeneticString::Vertex *c2,
    std::string sensor)
{
  std::map< std::string, std::string > letters_convertion;
  letters_convertion["C"] = "Core";
  letters_convertion["B"] = "FixedBrick";
  letters_convertion["AJ"] = "ActiveHinge";
  letters_convertion["ST"] = "TouchSensor";
  letters_convertion["SL"] = "LightSensor";

  // slots 0-back 1-front 2-right 3-left

  std::ofstream robot_file;
  std::string path = "../../experiments/" +
                     _directoryPath + "/robot_" + this->getId() + ".yaml";
  robot_file.open(
      path,
      std::ofstream::app);

  std::string letter_convertion = "";
  if (sensor == "")
  {
    letter_convertion = c2->item;
  }
  else
  {
    letter_convertion = sensor;
  }

  std::string spacing2 = "";

  // sets angles
  int angle = 0;
  if (letter_convertion == "AJ2")
  {
    if (parent_convertion == "AJ2")
    {
      angle = 0;
    }
    else
    {
      angle = 90;
    }
  }
  else
  {
    if (parent_convertion == "AJ2")
    { angle = -90; }
  }

  // creates spaces for hierarchy of yaml file
  for (int s = 1; s <= convertion_level; s++)
  {
    spacing2 += "    ";
  }


  if (letter_convertion == "C")
  {
    robot_file << "body:" << std::endl;
  }

  if (direction == "left")
  {
    robot_file << spacing2 << "3:" << std::endl;
  }
  if (direction == "front")
  {
    robot_file << spacing2 << "1:" << std::endl;
  }
  if (direction == "right")
  {
    robot_file << spacing2 << "2:" << std::endl;
  }
  if (direction == "root-back")
  { robot_file << spacing2 << "0:" << std::endl; }


  if (letter_convertion != "C")
  {
    robot_file << spacing2 << "  slot        : 0" << std::endl;
    robot_file << spacing2 << "  orientation : "
                              + std::to_string(angle) << std::endl;
  }

  if (letter_convertion.substr(
      0,
      2) == "AJ")
  {
    letter_convertion = "AJ";
  }


  robot_file << spacing2 << "  type        : "
                                "" + letters_convertion[letter_convertion]
             << std::endl;
  robot_file << spacing2 << "  id          : "
                                "module" + std::to_string(c2->id);

  if (sensor != "")
  {
    if (direction == "root-back")
    {
      robot_file << "sensor-b";
    }
    else
    {
      robot_file << "sensor-" << direction.substr(
          0,
          1);
    }
  }

  robot_file << std::endl;


  if (sensor == "" and
      (c2->left != NULL or c2->front != NULL or c2->right != NULL
       or c2->sensor_left != "Sn" or c2->sensor_front != "Sn"
       or c2->sensor_right != "Sn" or c2->sensor_back != "Sn"))
  {
    robot_file << spacing2 << "  children    :" << std::endl;
  }

  robot_file.close();
}


/**
*  Creates the genome of an individual in its initial state, as a genetic-string formed only by the axiom.
**/
void Genome::createEmbryo()
{

  std::vector< std::string > axiom;
  axiom.push_back("C");

  // initializes the genetic-string with the axiom
  GeneticString  gs =  GeneticString();
  this->setGeneticString(
      this->build_genetic_string(
          gs,
          axiom));

}


/**
*  Develops the initial genetic-string according to the grammar and creates phenotype.
* @param argc - command line parameter
* @param argv[] - command line parameter
* @param params - list of params read from configuration file.
* @param LS - Lsystem structure containing the alphabet.
**/

void Genome::developGenomeIndirect(
    int argc,
    char *argv[],
    std::map< std::string, double > params,
    LSystem LS,
    int generation,
    std::string path)
{
  // creates main genetic-string for axiom (initial developmental state of the genome)
  this->createEmbryo();

  // enhances the genetic-string according to grammar iteratively
  this->generate_final_string((int) params["replacement_iterations"],
                              (int) params["export_genomes"],
                              generation,
                              path);

  // decodes the final genetic-string into a tree of components
  this->decodeGeneticString(
      LS,
      params,
      path + std::to_string(generation));

  // generates robot-graphics
  this->constructor(
      argc,
      argv,
      params,
      path + std::to_string(generation));
}


void Genome::updateFitness(double fitness)
{
  this->fitness = fitness;
}


std::map< std::string, GeneticString  > Genome::getGrammar()
{
  return this->grammar;
}


void Genome::addLetterGrammar(
    std::string letter,
    GeneticString  lgs)
{
  this->grammar.emplace(
      letter,
      lgs);
}


void Genome::setValid(int valid)
{
  this->valid = valid;
}

int Genome::getValid()
{
  return this->valid;
}