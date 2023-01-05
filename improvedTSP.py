# Python3 implementation of the above approach
from random import randint

INT_MAX = 2147483647
# Number of cities in TSP
V = 5

# Names of the cities
GENES = "ABCDE"

# Starting Node Value
START = 0

# Initial population size for the algorithm
POP_SIZE = 10

# Structure of a GNOME
# defines the path traversed
# by the salesman while the fitness value
# of the path is stored in an integer

mp = [
    [0, 2, INT_MAX, 12, 5],
    [2, 0, 4, 8, INT_MAX],
    [INT_MAX, 4, 0, 3, 3],
    [12, 8, 3, 0, 10],
    [5, INT_MAX, 3, 10, 0],
]


class individual:
    def __init__(self) -> None:
        self.gnome = ""
        self.fitness = 0

    def __lt__(self, other):
        return self.fitness < other.fitness

    def __gt__(self, other):
        return self.fitness > other.fitness


def rand_num(start, end):
    '''
    Function to return a random number
    from start and end.
    '''
    return randint(start, end-1)


def repeat(s, ch):
    '''
    Function to check if the character
    has already occurred in the string
    '''
    for i in range(len(s)):
        if s[i] == ch:
            return True

    return False


def mutatedGene(gnome):
    '''
    Function to return a mutated GNOME
    Mutated GNOME is a string
    with a random interchange
    of two genes to create variation in species
    '''
    gnome = list(gnome)
    r, r1 = 0, 0
    while r1 == r:
        r = rand_num(1, V)
        r1 = rand_num(1, V)
    temp = gnome[r]
    gnome[r] = gnome[r1]
    [r1] = temp
    return ''.join(gnome)


# Function to return a valid GNOME string
# required to create the population
def create_gnome():
    gnome = "0"
    while len(gnome) != V:
        temp = rand_num(1, V)
        if not repeat(gnome, chr(temp + ord('0'))):
            gnome += chr(temp + ord('0'))
    gnome += gnome[0]

    return gnome


def cal_fitness(gnome):
    '''
    Function to return the fitness value of a gnome.
    The fitness value is the path length
    of the path represented by the GNOME.
    '''
    f = 0
    for i in range(len(gnome) - 1):
        if mp[ord(gnome[i]) - ord('0')][ord(gnome[i + 1]) - ord('0')] == INT_MAX:
            return INT_MAX
        f += mp[ord(gnome[i]) - ord('0')][ord(gnome[i + 1]) - ord('0')]

    return f


def cooldown(temp):
    '''
    Function to return the updated value of the cooling element.
    '''
    return (90 * temp) / 100


def TSPUtil():
    '''
    Utility function for TSP problem.
    '''
    # Generation Number
    gen = 1
    # Number of Gene Iterations
    gen_thres = 5

    population = []
    temp = individual()

    # Populating the GNOME pool.
    for i in range(POP_SIZE):
        temp.gnome = create_gnome()
        temp.fitness = cal_fitness(temp.gnome)
        population.append(temp)

    print("\nInitial population: \nGNOME	 FITNESS VALUE\n")
    for i in range(POP_SIZE):
        print(population[i].gnome, population[i].fitness)
    print()

    found = False
    temperature = 10000

    # Iteration to perform
    # population crossing and gene mutation.
    while temperature > 1000 and gen <= gen_thres:
        population.sort()
        print("\nCurrent temp: ", temperature)
        new_population = []

        for i in range(POP_SIZE):
            p1 = population[i]

            while True:
                new_g = mutatedGene(p1.gnome)
                new_gnome = individual()
                new_gnome.gnome = new_g
                new_gnome.fitness = cal_fitness(new_gnome.gnome)

                if new_gnome.fitness <= population[i].fitness:
                    new_population.append(new_gnome)
                    break

                else:

                    # Accepting the rejected children at
                    # a possible probability above threshold.
                    prob = pow(
                        2.7,
                        -1
                        * (
                            (float)(new_gnome.fitness - population[i].fitness)
                            / temperature
                        ),
                    )
                    if prob > 0.5:
                        new_population.append(new_gnome)
                        break

        temperature = cooldown(temperature)
        population = new_population
        print("Generation", gen)
        print("GNOME	 FITNESS VALUE")

        for i in range(POP_SIZE):
            print(population[i].gnome, population[i].fitness)
        gen += 1


if __name__ == "__main__":
    TSPUtil()
