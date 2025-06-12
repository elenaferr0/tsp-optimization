#include "genetic_algorithm/initialization/random_initialization.h"

#include <algorithm>
#include <random>
#include <set>

#include "genetic_algorithm/chromosome/chromosome.h"
#include "utils/maths.h"

RandomInitialization::RandomInitialization(Logger::Level log_level, Graph graph, int population_size)
    : PopulationInitialization(log_level, graph, population_size) {}

vector<Chromosome> RandomInitialization::generate_population() {
  int chromosome_size = graph.path.size();
    vector<int> indexes(chromosome_size);
    iota(indexes.begin(), indexes.end(), 0); // Fill with incrementing indices

    vector<Chromosome> population;
    for (int i = 0; i < population_size; ++i) {
        shuffle(indexes.begin(), indexes.end(), mt19937(random_device()()));
        vector<Node> tour;
        for (int index : indexes) {
            tour.push_back(graph.path[index]);
        }
        Chromosome ch((Graph(tour)));
        population.push_back(ch);
    }

    return population;
}