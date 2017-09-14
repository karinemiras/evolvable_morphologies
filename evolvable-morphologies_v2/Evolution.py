from GenomeIndirect import *

class Evolution:


    def __init__(self):

        self.params = {}
        self.population = []
        self.next_id = 1



    # Reads parameters from file.
    def readParams(self):

        # Reads parameters file.

        #    pop_size - size of the population of genomes
        #    offspring_prop - proportion of the population size to dcalculate size of offspring
        #    num_initial_comp - number of initial (random) components in the production rules of the grammar
        #    show_phenotypes - flag to show the phenotype graphic
        #    export_phenotypes - if exports the phenotypes to images (1) or not (0)
        #    export_genomes - if exports the genomes to files (1) or not (0)
        #    replacement_iterations - number of replacement iterations for the l-system
        #    size_component - size of each component in pixels
        #    spacing - spacing between components in pixels
        #    num_generations - number of generations of the evolution
        #    mutation_prob - probability of adding/removing/swaping items (letters/commands) to the genetic-string in the mutation
        #    max_comps - maximum number of components allowed per phenotype
        #    prob_add_archive - probability of adding any genome to the archive
        #    k_neighbors - number of neighbords to compare for fitness
        #    logs_to_screen - if exports the logs to the screen (1) or not (0)
        #    logs_to_file - if exports logs to a file (1) or not (0)


        file = open('configuration.txt', 'r')
        for line in file:
            self.params[line.split(' ')[0]] = line.split(' ')[1].split('\n')[0]



    # reads parameters for new experiment and creates directories
    def setupEvolution(self):

        self.readParams()



    def iniExperiment(self):

        self.setupEvolution()
        self.initPopulation()


    # Initializes the population with new genomes.
    def initPopulation(self):

        # creates new genomes according to population size
        for i in range(1, int(self.params['pop_size'])+1):

            # creates new genome
            gen = GenomeIndirect(self.next_id, self.params, 'N', 'N')
            gen.build_grammar()

            for k, v in gen.grammar.items():
                print k, v

            gen.develop()

            # adds genome to the population
            self.population.append(gen)

            # increments id that will be used for any next genome created
            self.next_id = self.next_id+1

