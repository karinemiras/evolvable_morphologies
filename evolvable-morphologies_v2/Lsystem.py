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
        self.alphabet.append('AJ ')  # active joint
        self.alphabet.append('B')    # brick

    # Builds a list with the possible mounting commands for the l-system.
    def build_mounting_commands(self):

        self.mounting_commands.append('addl') # add component to the left side of the parent-reference
        self.mounting_commands.append('addf') # add component to the front side of the parent-reference
        self.mounting_commands.append('addr') # add component to the right side of the parent-reference


    # Builds a list with the possible moving commands for the l-system.
    def build_moving_commands(self):

        self.moving_commands.append('moveb') # move back to parent in the turtle
        self.moving_commands.append('movel') # move to the left in the turtle
        self.moving_commands.append('movef') # move forward in the turtle
        self.moving_commands.append('mover') # move to the right in the turtle