#include "genetic_algorithm/initialization/random_initialization.h"

#include <algorithm>
#include <random>
#include <set>

#include "genetic_algorithm/chromosome/chromosome.h"
#include "utils/maths.h"

RandomInitialization::RandomInitialization(const Logger::Level log_level, const Graph &graph)
    : PopulationInitialization(Logger(log_level, "RandomInitialization"), graph) {
}

vector<Chromosome> RandomInitialization::generate_population(const HyperParams &params) const {
    const int chromosome_size = static_cast<int>(graph.path.size());
    vector<int> indexes(chromosome_size);
    iota(indexes.begin(), indexes.end(), 0); // Fill with incrementing indices

    vector<Chromosome> population;
    const int to_generate = floor(params.convex_hull_random_init_ratio.second * params.population_size);
    for (int i = 0; i < to_generate; ++i) {
        shuffle(indexes.begin(), indexes.end(), mt19937(random_device()()));
        vector<Node> tour;
        for (const int index: indexes) {
            tour.push_back(graph.path[index]);
        }
        Chromosome ch((Graph(tour)));
        population.push_back(ch);
    }

    return population;
}
