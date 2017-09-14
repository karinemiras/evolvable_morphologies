from Lsystem import *

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