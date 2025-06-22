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

void run_grid_search(const char *instance) {
    const auto mutation_rates = vector<double>{0.01, 0.05, 0.1};
    const auto parent_replacement_rates = vector<double>{0.1, 0.2, 0.3};
    const auto selection_tournament_sizes = vector<int>{5, 10, 15};
    const auto convex_hull_random_init_ratio = vector<pair<double, double> >{
        {0.2, 0.8}, // 20% convex hull, 80% random
    };

    const GridSearch gs(
        instance,
        mutation_rates,
        parent_replacement_rates,
        selection_tournament_sizes,
        convex_hull_random_init_ratio
    );
    gs.run();
}

void solve(const char *instance_name, const Logger::Level log_level = Logger::Level::INFO) {
    /*
      Params found through grid search:
          Selection: LinearRankingSelection
          Crossover: OrderCrossover
          Mutation: DisplacementMutation [rate: 0.100000]
          Replacement: ElitismReplacement [rate: 0.300000]
          Tournament size: 15
          Convex hull init ratio: 0.200000
          Random init ratio: 0.800000
     */
    auto graph = Graph(instance_name);

    constexpr auto params = HyperParams{
        .mutation_rate = 0.1,
        .parents_replacement_rate = 0.3,
        .selection_tournament_size = 15,
        .convex_hull_random_init_ratio = {0.2, 0.8} // 20% convex hull, 80% random
    };

    const auto population_init = vector<shared_ptr<PopulationInitialization> >{
        make_shared<ConvexHullInitialization>(log_level, graph),
        make_shared<RandomInitialization>(log_level, graph)
    };
    unique_ptr<SelectionOp> selection = make_unique<LinearRankingSelection>(log_level);
    unique_ptr<CrossoverOp> crossover = make_unique<OrderCrossover>(log_level);
    unique_ptr<MutationOp> mutation = make_unique<DisplacementMutation>(log_level);
    unique_ptr<Replacement> replacement = make_unique<ElitismReplacement>(log_level);
    const auto stopping_criteria = vector<shared_ptr<StoppingCriterion> >{
        make_shared<MaxNonImprovingGenerationsCriterion>(log_level),
        make_shared<TimeLimitCriterion>(log_level)
    };

    GeneticAlgorithm ga(
        population_init,
        (std::move(selection)),
        (std::move(crossover)),
        (std::move(mutation)),
        (std::move(replacement)),
        stopping_criteria,
        log_level
    );

    ga.start(params);
}

int main(const int argc, char *argv[]) {
    try {
        char *instance = nullptr;
        parse_params(argc, argv, instance);
        // run_grid_search(instance);
        solve(instance);
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
