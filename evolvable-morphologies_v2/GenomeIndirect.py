from Lsystem import *
from Vertex import *

from random import randint

class GenomeIndirect:


    def __init__(self, id, params, id_parent1, id_parent2 ):

        self.params = params
        self.grammar = dict()
        self.id = id
        self.id_parent1 = id_parent1
        self.id_parent2 = id_parent2
        self.gs = ['C'] # axiom
        self.ls =  Lsystem()


    # Generates initial production rules for the alphabet.
    def build_grammar(self):


        # for each letter of the alphabet
        for l in self.ls.alphabet:

            # production rule
            items = []

            # if it is a core component, forces its inclusion in the rule
            if l == 'C':
                items.append('C')

            # while a raffled number of components is not achieved
            # (times 3 because it must take the commands of type 'mounting' and 'moving' into account)
            while len(items) < randint(1, int(self.params['num_initial_comp']) *3):

                # raffles a letter to be included
                letter = self.ls.alphabet[randint(0, int(len(self.ls.alphabet)-1))]

                # prevents core component of being (re)included in the rule
                if letter != 'C':

                    # raffles a mounting command to be included
                    items.append(self.ls.mounting_commands[randint(0, int(len(self.ls.mounting_commands) - 1))])
                    # adds letter
                    items.append(letter)
                    # a raffles a moving command to be included
                    items.append(self.ls.moving_commands[randint(0, int(len(self.ls.moving_commands) - 1))])


            # adds letter and its production rule to the grammar of the genome
            self.grammar[l] = items


    def develop(self):

        self.generateFinalString()
        self.decodeGs()


    def generateFinalString(self):

        # performs replacements for a number of iterations
        for r in range(0, int(self.params['replacement_iterations'])):

            # for each letter in the geneticstring
            auxi = 0
            for i in range(0, len(self.gs)):

                temp_item = self.gs[auxi]

                if  temp_item in self.ls.alphabet:

                    # removes the letter
                    self.gs.pop(auxi)
                    # replaces for the producrtion rule
                    for ii in range(0, len(self.grammar[temp_item])):
                        self.gs.insert(auxi+ii,self.grammar[temp_item][ii])
                    auxi = auxi+ii+1

                else:
                    auxi = auxi +1
            print self.gs


    # Transforms the main genetic-string of the genome into a graph that logically represents the connections among the components.
    def decodeGs(self):

        num_components = 0
        mountingcommand = ''
        root = None
        violation = False

        #  for each item of the main genetic-string
        for i in range(0, len(self.gs)):

            # if limit number of components has not been reached
            if num_components < int(self.params['max_comps'])-1:

                # if the item is a letter (component) in the alphabet
                if self.gs[i] in self.ls.alphabet:

                    new_component = Vertex(self.gs[i])

                    # if there are no components in the graph yet, creates root
                    if root == None:

                        new_component.back = None
                        current_component = new_component
                        root = new_component
                        mountingcommand = ''

                    # if the graph has been started already
                    else:

                        #    if a command has been set for mounting the component
                        #    commands of previous vaiolations remain to pair with a possible new letter, unless a new command be added
                        if mountingcommand:

                            component =  current_component.item[0:1]
                            #if component is core or brick, having multilateral connection
                            if component == 'C' or component=='B':

                                # if all sides are occupied in the root, grows to the back
                                if root == current_component and current_component.left != None and current_component.front != None and current_component.right != None:

                                    if current_component.back == None:

                                        current_component.back = new_component
                                    else:
                                        violation = True

                            # mounts component in the sue side if theres no violation
                            else:

                                if (mountingcommand == "l"): # mounts component on the left
                                    if (current_component.left != None): # if position is occupied

                                        violation = True
                                    else:
                                        current_component.left = new_component

                                if (mountingcommand == "f"): #  mounts component on the front
                                    if (current_component.front != None): # if position is occupied

                                        violation = True
                                    else:
                                        current_component.front = new_component

                                if (mountingcommand == "r"): # mounts component on the right
                                    if (current_component.right != None): # if position is occupied

                                        violation = True
                                    else:
                                        current_component.right = new_component


                        else: # if component is a joint

                            if (current_component.front != None): # if position is occupied, then its a violation

                                violation = True
                            else:
                                current_component.front = new_component  # adds component to the front of the joint

                        #  when the genetic-string leads to trying to hatch a component into a position where theres already another component or a sensor, the command is ignored
                        if violation == False:

                            new_component.back = current_component
                            current_component = new_component
                            num_components = num_components + 1


                else:

                    typecommand = self.gs[i][0:4]

                    # if it is a moving command
                    if (typecommand == "move"):

                        movingcommand = self.gs[i][4:1]

                        if (movingcommand == "b"):

                            # if it is not root, moves back to parent component in the graph
                            if (current_component != root):
                                    current_component = current_component.back


                        if (movingcommand == "l"):

                            # if there is a component to the left, moves to it in the graph
                            if (current_component.left != None):
                                current_component = current_component.left


                        if (movingcommand == "f"):

                            # if there is a component to the front, moves to it in the graph
                            if (current_component.front != None):
                                current_component = current_component.front

                            if (movingcommand == "r"):

                                # if there is a component to the left, moves to it in the graph
                                if (current_component.right != None):
                                    current_component = current_component.right


                    # if it is a mounting command
                    else:

                        #  discovers the type of mounting command, to be used with the next component to be mounted later on
                        mountingcommand = self.gs[i][3:1]




    # Draws a chart from the graph.
   # def constructor(self):

    #    self.draw_component(self.)


