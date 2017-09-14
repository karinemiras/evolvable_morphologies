#  Alphabet containing the letters and the commands com the l-system.

class Lsystem:



    def __init__(self):

        self.alphabet = [] # alphabet with the letters
        self.mounting_commands = [] # vector with the commands to mount components in the turtle
        self.moving_commands = [] # vector with the commands to move in the turtle

        self.build_alphabet()
        self.build_mounting_commands()
        self.build_moving_commands()

    # Builds a list with the possible letters of the alphabet of the l-system.
    def build_alphabet(self):

        self.alphabet.append('C')    # core component
        self.alphabet.append('J')    # fixed joint
        self.alphabet.append('AJ1')  # active joint - horizontal
        self.alphabet.append('AJ2')  # active joint - vertical
        self.alphabet.append('PJ1')  # passive joint - horizontal
        self.alphabet.append('PJ2')  # passive joint - vertical
        self.alphabet.append('B')    # brick

    # Builds a list with the possible mounting commands for the l-system.
    def build_mounting_commands(self):

        self.mounting_commands.append('bd_addl') # add component to the left side of the parent-reference
        self.mounting_commands.append('bd_addf') # add component to the front side of the parent-reference
        self.mounting_commands.append('bd_addr') # add component to the right side of the parent-reference


    # Builds a list with the possible moving commands for the l-system.
    def build_moving_commands(self):

        self.moving_commands.append('bd_moveb') # move back to parent in the turtle
        self.moving_commands.append('bd_movel') # move to the left in the turtle
        self.moving_commands.append('bd_movef') # move forward in the turtle
        self.moving_commands.append('bd_mover') # move to the right in the turtle