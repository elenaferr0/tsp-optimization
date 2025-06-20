#include <iostream>
#include <memory>

#include "tsp/graph.h"
#include <utils/path.h>

#include "genetic_algorithm/genetic_algorithm.h"
#include "genetic_algorithm/initialization/random_initialization.h"
#include "genetic_algorithm/initialization/convex_hull_initialization.h"
#include "genetic_algorithm/selection/linear_ranking_selection.h"
#include "genetic_algorithm/crossover/order_crossover.h"
#include "genetic_algorithm/crossover/edge_recombination_crossover.h"
#include "genetic_algorithm/replacement/elitism_replacement.h"
#include "genetic_algorithm/stopping/max_non_improving_generations_criterion.h"
#include "genetic_algorithm/stopping/time_limit_criterion.h"
#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "genetic_algorithm/replacement/steady_state_replacement.h"
#include "genetic_algorithm/grid_search.h"

using namespace std;

void parse_params(const int argc, char *argv[], char *&instance) {
    if (argc < 1) {
        std::cerr << "Usage: " << argv[0] << " <instance_name>" << std::endl;
        exit(1);
    }

    instance = argv[1];
    if (instance == nullptr || instance[0] == '\0') {
        throw std::invalid_argument("Instance name must be provided as the first argument.");
        exit(1);
    }
}

int main(const int argc, char *argv[]) {
    // char *instance = nullptr;
    // parse_params(argc, argv, instance);

    try {
        const auto mutation_rates = vector<double>{0.01, 0.05, 0.1};
        // const auto mutation_rates = vector<double>{0.01};
        const auto parent_replacement_rates = vector<double>{0.1, 0.2, 0.3};
        // const auto parent_replacement_rates = vector<double>{0.3};
        const auto selection_tournament_sizes = vector<int>{5, 10, 15};
        // const auto selection_tournament_sizes = vector<int>{15};
        const auto convex_hull_random_init_ratio = vector<pair<double, double> >{
            // {0.1, 0.9}, // 10% convex hull, 90% random
            {0.2, 0.8}, // 20% convex hull, 80% random
            // {0.8, 0.2}, // 80% convex hull, 20% random
            // {0.5, 0.5} // 50% convex hull, 50% random
        };

        const GridSearch gs(
            "./samples/random_10.dat",
            mutation_rates,
            parent_replacement_rates,
            selection_tournament_sizes,
            convex_hull_random_init_ratio
        );
        gs.run();

    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
