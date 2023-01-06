// C++ implementation of the above approach
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

// Number of cities in TSP
#define V 5

// Names of the cities
#define GENES ABCDE

// Starting Node Value
#define START 0

// Initial population size for the algorithm
#define POP_SIZE 10

int map[V][V] = {{0, 2, INT_MAX, 12, 5},
                 {2, 0, 4, 8, INT_MAX},
                 {INT_MAX, 4, 0, 3, 3},
                 {12, 8, 3, 0, 10},
                 {5, INT_MAX, 3, 10, 0}};

// Structure of a GNOME
// string defines the path traversed
// by the salesman while the fitness value
// of the path is stored in an integer

struct individual {
    std::string gnome;
    int fitness;
};

// Function to return a random number
// from start and end
int rand_num(int start, int end) {
    int r = end - start;
    int rnum = start + rand() % r;
    return rnum;
}

// Function to check if the character
// has already occurred in the string
bool repeat(std::string s, char ch) {
    for (char currentChar : s) {
        if (currentChar == ch)
			return true;
    }
    return false;
}

// Function to return a mutated GNOME
// Mutated GNOME is a string
// with a random interchange
// of two genes to create variation in species
std::string mutatedGene(std::string gnome) {
    int r, r1;
    do {
        r = rand_num(1, V);
        r1 = rand_num(1, V);
    } while (r1 == r);
    char temp = gnome[r];
    gnome[r] = gnome[r1];
    gnome[r1] = temp;
    return gnome;
}

// Function to return a valid GNOME string
// required to create the population
std::string create_gnome() {
    std::string gnome = "0";
    while (gnome.size() != V) {
        int temp = rand_num(1, V);
        if (!repeat(gnome, (char)(temp + '0')))
            gnome += (char)(temp + '0');
    }
    gnome += gnome[0];
    return gnome;
}

// Function to return the fitness value of a gnome.
// The fitness value is the path length
// of the path represented by the GNOME.
int cal_fitness(std::string gnome) {
    int f = 0;
    for (char gene : gnome) {
        if (map[gene - '0'][gene - '0'] == INT_MAX)
            return INT_MAX;
        f += map[gene - '0'][gene - '0'];
    }
    return f;
}

// Function to return the updated value
// of the cooling element.
int cooldown(int temp) {
    return (90 * temp) / 100;
}

// Comparator for GNOME struct.
bool lessthan(struct individual t1,
              struct individual t2) {
    return t1.fitness < t2.fitness;
}

// Utility function for TSP problem.
void TSPUtil(int map[V][V]) {
    // Generation Number
    int gen = 1;
    // Number of Gene Iterations
    int gen_thres = 5;

    std::vector<struct individual> population;
    struct individual temp;

    // Populating the GNOME pool.
    for (int i = 0; i < POP_SIZE; i++) {
        temp.gnome = create_gnome();
        temp.fitness = cal_fitness(temp.gnome);
        population.push_back(temp);
    }

    std::cout << "\nInitial population: \n"
              << "GNOME	 FITNESS VALUE\n";
    for (int i = 0; i < POP_SIZE; i++)
        std::cout << population[i].gnome << " "
                  << population[i].fitness << '\n';
    std::cout << "\n";

    int temperature = 10000;

    // Iteration to perform
    // population crossing and gene mutation.
    while (temperature > 1000 && gen <= gen_thres) {
        sort(population.begin(), population.end(), lessthan);
        std::cout << "\nCurrent temp: " << temperature << "\n";
        std::vector<struct individual> new_population;

        for (int i = 0; i < POP_SIZE; i++) {
            struct individual p1 = population[i];

            while (true) {
                std::string new_g = mutatedGene(p1.gnome);
                struct individual new_gnome;
                new_gnome.gnome = new_g;
                new_gnome.fitness = cal_fitness(new_gnome.gnome);

                if (new_gnome.fitness <= population[i].fitness) {
                    new_population.push_back(new_gnome);
                    break;
                } else {
                    // Accepting the rejected children at
                    // a possible probability above threshold.
                    float prob = pow(2.7,
                                     -1 * ((float)(new_gnome.fitness - population[i].fitness) / temperature));
                    if (prob > 0.5) {
                        new_population.push_back(new_gnome);
                        break;
                    }
                }
            }
        }

        temperature = cooldown(temperature);
        population = new_population;
        std::cout << "Generation " << gen << " \n";
        std::cout << "GNOME	 FITNESS VALUE\n";

        for (int i = 0; i < POP_SIZE; i++)
            std::cout << population[i].gnome << " "
                      << population[i].fitness << "\n";
        gen++;
    }
}

int main() {
    TSPUtil(map);
    return 0;
}
