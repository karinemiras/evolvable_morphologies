//
// Created by Karine Miras on 08/03/2017.
// Created by Karine Miras on 08/03/2017.
//


#include <iostream>
#include <fstream>
#include <random>
#include <stdlib.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "DecodedGeneticString.h"
#include "GeneticString.h"
#include "LSystem.h"


/**
 * Transforms the main genetic-string of the genome into graphs that
 * logically represent the connections among the components of body and brain.
 * @param gs - main genetic-string of the genome.
 * @param LS - Lsystem structure containing the alphabet.
 * @param params - parameters of the system
 */
void DecodedGeneticString::decode(GeneticString gs,
                                  LSystem LS,
                                  std::map<std::string, double> params,
                                  std::string path)
{


    std::ofstream file;
    file.open("../../experiments/"+ path +"/tempbrain.dot");
    file<<" digraph g{ forcelabels=true;"<<std::endl;
    file.close();

    int num_components = 0;
    std::string mountingcommand = "";
    DecodedGeneticString::Vertex  *current_component = NULL;
    DecodedGeneticString::Vertex *new_component = NULL;
    GeneticString::Node *current_gs_item;
    current_gs_item = gs.getStart();

    // for each item of the main genetic-string
    for (int i = 0; i < gs.count(); i++)
    {

        // if there are brain parameters for the letter (joint)
        std::vector<std::string> tokens;
        boost::split(tokens, current_gs_item->item, boost::is_any_of("_"));
        std::string aux_item;
        if(tokens.size()>1) aux_item = tokens[0];
        else aux_item = current_gs_item->item;


        // if the item is a letter (component) in the alphabet
        if (LS.getAlphabet().count(aux_item) > 0){

            // if limit number of components has not been reached
            if(num_components < params["max_comps"])
            {

                // if item is a sensor
                if (LS.getAlphabetType()[aux_item] == "sensor") {
                    if (root != NULL // if previous component is core or brick
                        and (current_component->item == "C" or
                             current_component->item == "B")) {
                        // if there's a mounting command
                        if (!mountingcommand.empty()) {

                            // if all sides but back are occupied
                            if (root == current_component
                                and (current_component->left != NULL or
                                     current_component->sensor_left != "Sn")
                                and (current_component->front != NULL or
                                     current_component->sensor_front != "Sn")
                                and (current_component->right != NULL or
                                     current_component->sensor_right != "Sn")
                                    ) {
                                if (current_component->back == NULL and
                                    current_component->sensor_back == "Sn") {
                                    current_component->sensor_back = aux_item;
                                    this->decodeBrainNode
                                            ("b",
                                             current_gs_item->item,
                                             current_component->id, path);
                                }
                            } else {

                                if (mountingcommand == "l") {
                                    if (current_component->left == NULL and
                                        current_component->sensor_left ==
                                        "Sn") {
                                        current_component->sensor_left = aux_item;
                                        this->decodeBrainNode
                                                (mountingcommand,
                                                 current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                                if (mountingcommand == "f") {
                                    if (current_component->front == NULL and
                                        current_component->sensor_front ==
                                        "Sn") {
                                        current_component->sensor_front = aux_item;
                                        this->decodeBrainNode
                                                (mountingcommand,
                                                 current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                                if (mountingcommand == "r") {
                                    if (current_component->right == NULL and
                                        current_component->sensor_right ==
                                        "Sn") {
                                        current_component->sensor_right = aux_item;
                                        this->decodeBrainNode
                                                (mountingcommand,
                                                 current_gs_item->item,
                                                 current_component->id, path);
                                    }
                                }
                            }

                        }
                    }
                }

                // if item is a body component
                if (LS.getAlphabetType()[aux_item] == "body") {

                    std::string letter = aux_item;

                    // creates new node in the graph with the current letter (component)
                    new_component = new DecodedGeneticString::Vertex;
                    new_component->item = letter;
                    new_component->id = num_components;
                    new_component->left = NULL;
                    new_component->front = NULL;
                    new_component->right = NULL;

                    // if there are no components in the graph yet, creates root
                    if (root == NULL) {

                        new_component->back = NULL;
                        current_component = new_component;
                        num_components++;
                        root = new_component;
                        mountingcommand = "";

                    } else { // if the graph has been started already

                        // if a command has been set for mounting the component
                        // commands of previous violations remain to pair with a possible new letter, unless a new command be added
                        if (!mountingcommand.empty()) {

                            std::string component = current_component->item.substr(0, 1);

                            // if component is core or brick, having multilateral connection
                            if (component == "C" or component == "B") {


                                // if all sides are occupied in the root, grows to the back
                                if (root == current_component
                                    and (current_component->left != NULL or
                                         current_component->sensor_left != "Sn")
                                    and (current_component->front != NULL or
                                         current_component->sensor_front !=
                                         "Sn")
                                    and (current_component->right != NULL or
                                         current_component->sensor_right !=
                                         "Sn")
                                        ) {

                                    if (current_component->back == NULL and
                                        current_component->sensor_back ==
                                        "Sn") {

                                        current_component->back = new_component;
                                    } else {

                                        goto violation;
                                    }

                                } else { // mounts component in the sue side if theres no violation

                                    if (mountingcommand ==
                                        "l") {  // mounts component on the left
                                        if (current_component->left != NULL
                                            or current_component->sensor_left !=
                                               "Sn") { // if position is occupied

                                            goto violation;
                                        } else {

                                            current_component->left = new_component;
                                        }
                                    }

                                    if (mountingcommand ==
                                        "f") {  // mounts component on the front
                                        if (current_component->front != NULL
                                            or
                                            current_component->sensor_front !=
                                            "Sn") { // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->front = new_component;
                                        }
                                    }

                                    if (mountingcommand ==
                                        "r") {  // mounts componentf on the
                                      // right
                                        if (current_component->right != NULL
                                            or
                                            current_component->sensor_right !=
                                            "Sn") {  // if  position is occupied

                                            goto violation;
                                        } else {

                                            current_component->right = new_component;
                                        }
                                    }
                                }


                            } else {

                                // if current-component(parent) is a joint
                                // (always mounts on the front)
                                if (component == "A") {

                                    if (current_component->front !=
                                        NULL) {  // if position is occupied, then its a violation

                                        goto violation;
                                    } else {

                                        current_component->front = new_component; // adds component to the front of the joint
                                    }
                                }


                            }

                            new_component->back = current_component;
                            current_component = new_component;
                            num_components++;
                            mountingcommand = "";

                            if (new_component->item.substr(0, 1) == "A") {
                                this->decodeBrainNode("", current_gs_item->item,
                                                      new_component->id, path);
                            }

                            violation:
                            int vio = 1;// when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored


                        }
                    }
                }
            }

        // the item is a command
        } else {

            std::string typecommand = current_gs_item->item.substr(0, 3);

            // if it is a moving command
            if (typecommand == "mov") {


                std::string movingcommand = current_gs_item->item.substr(4, 1);

                // if new component is a joint
                // left/front/right move to the front
                if(current_component->item.substr(0, 1) == "A"){

                    if (movingcommand == "b") {
                        current_component = current_component->back;
                    }else {

                        if (current_component->front != NULL) {

                            current_component = current_component->front;
                        }
                    }

                }else {

                    if (movingcommand == "b") {
                        // if it is not root, moves back to parent component in the graph
                        if (current_component != root) {

                            current_component = current_component->back;
                        }
                    }

                    if (movingcommand == "l") {
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->left != NULL) {

                            current_component = current_component->left;
                        }
                    }

                    if (movingcommand == "f") {
                        // if there is a component to the front, moves to it in the graph
                        if (current_component->front != NULL) {

                            current_component = current_component->front;
                        }
                    }

                    if (movingcommand == "r") {
                        // if there is a component to the left, moves to it in the graph
                        if (current_component->right != NULL) {

                            current_component = current_component->right;
                        }
                    }
                }

            }

            // if it is a mounting command
            if (typecommand == "add")
            {
                // discovers the type of mounting command, to be used with the next component to be mounted later on
                mountingcommand = current_gs_item->item.substr(3, 1);
            }

            // if it is a brain command
            if (typecommand == "bra")
            {
                this->decodeBrainCommand(current_gs_item->item, path);
            }


        }
        current_gs_item = current_gs_item->next;
    }


    // writes edges to visualization
    file.open("../../experiments/"+ path +"/tempbrain.dot", std::ofstream::app);
    for (const auto &c : this->brain_edges)
    {
        auto origin = c.first.first;
        auto destination = c.first.second;
        auto weight = c.second;
        file<<origin<<" -> "<< destination
            <<"[label=\""<<weight<<" \",fontsize=\"8\"];"<<std::endl;
    }
    file<<" }"<<std::endl;
    file.close();

}

/** Decodes commands of the alphabet to alter the brain graph.
 * @param item - item of the alphabet
 * @path - directory to salve files
 * */
void DecodedGeneticString::decodeBrainCommand(std::string item,
                                              std::string path)
{
    // if there is a current-edge
    if(this->toNode.size() != 0 and this->fromNode.size() != 0 )
    {
        std::ofstream file;
        file.open("../../experiments/"+ path +"/tempbrain.dot", std::ofstream::app);

        std::vector<std::string> tokens;
        boost::split(tokens, item,  boost::is_any_of("_"));

        auto command = tokens[0];
        auto param = tokens[1];

        auto edge = std::make_pair(
                this->fromNode[0]->id,
                this->toNode[0]->id);

        // pertubs weight of current-edge
        if(command == "brainperturb")
        {
            if(this->brain_edges.count(edge) > 0)
            {
                this->brain_edges[edge] = this->brain_edges[edge]
                                          + std::stod(param);
                if (this->brain_edges[edge] > 1) this->brain_edges[edge] = 1;
                if (this->brain_edges[edge] < -1) this->brain_edges[edge] = -1;
            }
        }

        // creates self-connection
        if(command == "brainloop")
        {
            auto self_edge = std::make_pair(
                    this->toNode[0]->id,
                    this->toNode[0]->id);
            // if self-connection does not exist already
            // and node is output
            if(this->brain_edges.count(self_edge) == 0
               and this->toNode[0]->layer != "hidden")
            {
                this->brain_edges[self_edge] = std::stod(param);
            }
        }

        // creates node
        if(command == "brainnode")
        {
            // if a connection exists, creates new node in between nodes
            if(this->brain_edges.count(edge) > 0)
            {
              DecodedGeneticString::Vertex2 * v = new Vertex2();
              this->ids++;
              v->id = this->ids;
              v->id_comp = 0;
              v->layer = "hidden";
              v->direction = "";

              tokens = boost::split(tokens, param, boost::is_any_of("|"));
              v->weight = std::stod(tokens[0]);
              v->function = tokens[1];

              if(v->function == "Oscillator")
              {
                v->amplitude = std::stod(tokens[2]);
                v->period = std::stod(tokens[3]);
                v->phase_offset = std::stod(tokens[4]);
              }
              else
                v->bias = std::stod(tokens[2]);


              // sets node in the visualization
              auto output_text = std::to_string(v->id)
                                +"[label=<"+std::to_string(v->id)+"<BR/>"
                                +v->function+"<BR/>";
              if(v->function == "Oscillator")
              {
                output_text += " amplitude: " + std::to_string(v->amplitude)+"<BR />";
                output_text += " period: " + std::to_string(v->period)+"<BR />";
                output_text += " phase_offset: " + std::to_string(v->phase_offset);
              }
              else
                output_text += " bias: " + std::to_string(v->bias);

              output_text +=">,shape=box,fontsize=8];";
              file << output_text<<std::endl;

              // updates list of nodes
              this->brain_nodes[v->id] = v;


              // removes old link
              double old_w = this->brain_edges[edge];
              this->brain_edges.erase(edge);

              // adds link from 'from' to new
              edge = std::make_pair(
                      this->fromNode[0]->id,
                      v->id);
              this->brain_edges[edge] = old_w;

              // adds link new in to 'to'
              edge = std::make_pair(
                      v->id,
                      this->toNode[0]->id);
              this->brain_edges[edge] = v->weight;

              // updates pointers of current-edge brain graph

              for (int i=0; i<this->fromNode[0]->to_nodes.size(); i++)
              {
                  if (this->fromNode[0]->to_nodes[i] == this->toNode[0])
                  {
                      this->fromNode[0]->to_nodes[i] = v;
                      v->from_nodes.push_back(this->fromNode[0]);
                  }
              }
              for (int i=0; i<this->toNode[0]->from_nodes.size(); i++)
              {
                  if (this->toNode[0]->from_nodes[i] == this->fromNode[0])
                  {
                      this->toNode[0]->from_nodes[i] = v;
                      v->to_nodes.push_back(this->toNode[0]);
                  }
              }

            }
        }

        // creates edge
        if(command == "brainedge")
        {
            // if there is no connection, creates edge
            if (this->brain_edges.count(edge) == 0)
            {
                this->fromNode[0]->to_nodes.push_back(this->toNode[0]);
                this->toNode[0]->from_nodes.push_back(this->fromNode[0]);

                edge = std::make_pair(
                        this->fromNode[0]->id,
                        this->toNode[0]->id);
                this->brain_edges[edge] = std::stod(param);


            }
        }

        // moves 'from' to parent
        if(command == "brainmoveFTP")
        {
            auto node = std::stod(param);
            // if it is not input layer
            if (this->fromNode[0]->from_nodes.size() != 0) {
                // arranges to which parent to go
                if (this->fromNode[0]->from_nodes.size() < node) {
                    node = this->fromNode[0]->from_nodes.size() - 1;
                } else node = node - 1;
                this->fromNode[0] = this->fromNode[0]->from_nodes[node];
            }
        }

        // moves 'from' to child
        if(command == "brainmoveFTC")
        {
            auto node = std::stod(param);
            // arranges to which child to go
            if(this->fromNode[0]->to_nodes.size() < node){
                node = this->fromNode[0]->to_nodes.size()-1;
            } else node = node-1;
            // if child is hidden node (not output)
            // and is not the current 'to'
            if(this->fromNode[0]->to_nodes[node]->layer == "hidden"
               and this->fromNode[0]->to_nodes[node] != this->toNode[0])
            {
                this->fromNode[0] = this->fromNode[0]->to_nodes[node];
            }
        }

        // moves 'from' to sibling
        if(command == "brainmoveFTS")
        {
            tokens = boost::split(tokens, tokens[1],  boost::is_any_of("|"));
            auto intermediate = std::stod(tokens[0]);
            auto sibling = std::stod(tokens[1]);

            // arranges to which intermediate to go
            if (this->fromNode[0]->to_nodes.size() < intermediate){
                intermediate = this->fromNode[0]->to_nodes.size() - 1;
            } else intermediate = intermediate - 1;

            // arranges to which sibling to go
            if (this->fromNode[0]->to_nodes[intermediate]->from_nodes.size() < sibling){
                sibling = this->fromNode[0]->to_nodes[intermediate]->from_nodes.size() - 1;
            } else sibling = sibling - 1;

            this->fromNode[0] =
                    this->fromNode[0]->to_nodes[intermediate]->from_nodes[sibling];
        }

        // moves 'to' to parent
        if(command == "brainmoveTTP")
        {
            auto node = std::stod(param);
            // arranges to which parent to go
            if(this->toNode[0]->from_nodes.size() < node){
                node = this->toNode[0]->from_nodes.size()-1;
            } else node = node-1;
            // if parent is hidden node (not input)
            // or is the current 'to'
            if(this->toNode[0]->from_nodes[node]->layer == "hidden"
               and this->toNode[0]->from_nodes[node] != this->fromNode[0]){
                this->toNode[0] = this->toNode[0]->from_nodes[node];
            }
        }

        // moves 'to' to child
        if(command == "brainmoveTTC")
        {
            auto node = std::stod(param);
            // if it is not output layer
            if (this->toNode[0]->to_nodes.size() != 0) {
                // arranges to which child to go
                if (this->toNode[0]->to_nodes.size() < node) {
                    node = this->toNode[0]->to_nodes.size() - 1;
                } else node = node - 1;
                this->toNode[0] = this->toNode[0]->to_nodes[node];
            }
        }

        // moves 'to' to sibling
        if(command == "brainmoveTTS")
        {
            tokens = boost::split(tokens, tokens[1],  boost::is_any_of("|"));
            auto intermediate = std::stod(tokens[0]);
            auto sibling = std::stod(tokens[1]);

            // arranges to which intermediate to go
            if (this->toNode[0]->from_nodes.size() < intermediate){
                intermediate = this->toNode[0]->from_nodes.size() - 1;
            } else intermediate = intermediate - 1;

            // arranges to which sibling to go
            if (this->toNode[0]->from_nodes[intermediate]->to_nodes.size() < sibling){
                sibling = this->toNode[0]->from_nodes[intermediate]->to_nodes.size() - 1;
            } else sibling = sibling - 1;

            this->toNode[0] =
                    this->toNode[0]->from_nodes[intermediate]->to_nodes[sibling];
        }
        file.close();
    }
}

/** Decodes letters (sensors and joints) of the alphabet into part of the brain
 * graph.
 * @param item - item of the alphabet
 * @param id_comp - if of the body component to which this node is related to
 * @path - directory to salve files
 * */
void DecodedGeneticString::decodeBrainNode(std::string direction,
                                           std::string item,
                                           int id_comp,
                                           std::string path)
{
    std::ofstream file;
    file.open("../../experiments/"+ path +"/tempbrain.dot", std::ofstream::app);

    std::vector<std::string> tokens;
    boost::split(tokens, item,  boost::is_any_of("_"));
    item = tokens[0];
    auto param = tokens[1];

    DecodedGeneticString::Vertex2 * v = new Vertex2();
    this->ids++;
    v->id = this->ids;
    v->id_comp = id_comp;

    // the item is a sensor
    if(item == "ST" or item == "SL")
    {
        v->layer = "input";
        v->bias = 0;
        v->function = "Input";
        // direction is to identify the sensor, ex.: sensor of the left
        v->direction = direction;
        // if there's no output node yet
        // adds node to the list of 'from' nodes of current-edge
        if (this->toNode.size() == 0)
        {
            v->weight = std::stod(param);
            this->fromNode.push_back(v);
        }else
        {
            // update 'from' of current-edge
          if(this->fromNode.size()>0)
              this->fromNode[0] = v;
          else
            this->fromNode.push_back(v);

            // if there's a list, connects new node to them all and updates the list
            // with the latest node only
            for(int i=0; i< this->toNode.size(); i++)
            {
                // connects new node to 'to-node' of current edge
                this->toNode[i]->from_nodes.push_back(this->fromNode[0]);
                this->fromNode[0]->to_nodes.push_back(this->toNode[i]);

                std::pair< int, int > edge = std::make_pair(
                    this->fromNode[0]->id,
                    this->toNode[i]->id);

              if(i == this->toNode.size()-1)
                // weight of new node
                this->brain_edges[edge] = std::stod(param);
              else
                // weight of node on the stack
                this->brain_edges[edge] = this->toNode[i]->weight;
            }
            // remove from 'to' list after connection, except for the last
            this->toNode.erase(
                this->toNode.begin(),
                this->toNode.begin() + this->toNode.size() - 1);
        }

        // sets node in the visualization
        if (item == "SL") {
            file << v->id << "[label=<"<<v->id<<"<BR/>M"<<v->id_comp<<">"
                    ",color=\"lightgrey\",style=filled,fontsize=8];"
                    ""<<std::endl;
        }
        if (item == "ST") {
            file << v->id << "[label=<"<<v->id<<"<BR/>M"<<v->id_comp<<">"
                    ",color=\"grey\",style=filled,fontsize=8];"
                    ""<<std::endl;
        }

        // updates list of nodes
        this->brain_nodes[this->ids] = v;
    }

    // the item is an active joint
    if(item == "AJ1" or item == "AJ2")
    {
        v->layer = "output";
        v->direction = "";

        boost::split(tokens, param,  boost::is_any_of("|"));
        v->weight = std::stod(tokens[0]);
        v->function = tokens[1];
        if(v->function == "Oscillator")
        {
          v->amplitude = std::stod(tokens[2]);
          v->period = std::stod(tokens[3]);
          v->phase_offset = std::stod(tokens[4]);
        }
        else
          v->bias = std::stod(tokens[2]);

        // if there's no input node yet
        // adds node to the list of 'to' nodes of current-edge
        if (this->fromNode.size() == 0)
        {
            this->toNode.push_back(v);
        }else
        {
            // update 'to' of current-edge
          if(this->toNode.size() >0)
            this->toNode[0] = v;
          else
            this->toNode.push_back(v);

            // if theres a list, connects new node to them all and updates the list
            // with the latest node only
            for (int i = 0; i < this->fromNode.size(); i++)
            {
                // connects new node to 'from-node(s)' of current edge
                this->toNode[0]->from_nodes.push_back(this->fromNode[i]);
                this->fromNode[i]->to_nodes.push_back(this->toNode[0]);

                std::pair< int, int > edge = std::make_pair(
                    this->fromNode[i]->id,
                    this->toNode[0]->id);

              if(i == this->fromNode.size()-1)
                // weight of new node
                this->brain_edges[edge] = v->weight;
              else
                // weight of node on the stack
                this->brain_edges[edge] = this->fromNode[i]->weight;
            }

            // remove from 'from' list after connection, except for the last
            this->fromNode.erase(
                this->fromNode.begin(),
                this->fromNode.begin() + this->fromNode.size() - 1);
        }

        // sets node in the visualization
        auto output_text = std::to_string(v->id) + " [label=<"
                           +std::to_string(v->id) +"<BR />" +v->function
                           +" M"+std::to_string(v->id_comp)+"<BR />";
        if(v->function == "Oscillator")
        {
          output_text += " amplitude: " + std::to_string(v->amplitude)+"<BR />";
          output_text += " period: " + std::to_string(v->period)+"<BR />";
          output_text += " phase_offset: " + std::to_string(v->phase_offset);
        }
        else
          output_text += " bias: " + std::to_string(v->bias);

        output_text +=">, shape=box,color=red,style=filled,fontsize=8];";

        file << output_text<<std::endl;

        // updates list of nodes
        this->brain_nodes[this->ids] = v;
    }
    file.close();
}

/**
 * @return pointer to the root of the body graph
 */
DecodedGeneticString::Vertex * DecodedGeneticString::getRoot(){
    return this->root;
}


std::map< std::pair<int, int>, double > DecodedGeneticString::getBrain_edges(){
    return this->brain_edges;
}

std::map< int, DecodedGeneticString::Vertex2 *>
DecodedGeneticString::getBrain_nodes(){
        return this->brain_nodes;
}



