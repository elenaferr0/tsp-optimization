#include <iostream>
#include <memory>

#include "tsp/graph.h"
#include <utils/path.h>

#include "genetic_algorithm/genetic_algorithm.h"
#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "genetic_algorithm/initialization/convex_hull_initialization.h"
#include "genetic_algorithm/selection/linear_ranking_selection.h"
#include "genetic_algorithm/crossover/order_crossover.h"
#include "genetic_algorithm/replacement/steady_state_replacement.h"
#include "genetic_algorithm/selection/n_tournament_selection.h"
#include "genetic_algorithm/stopping/max_non_improving_generations_criterion.h"
#include "genetic_algorithm/stopping/time_limit_criterion.h"

class SelectionOp;
using namespace std;

int main(const int argc, char *argv[]) {
    try {
        auto level = Logger::Level::DEBUG;

        unique_ptr<PopulationInitialization> chromosomes = make_unique<ConvexHullInitialization>(level, Graph::from_file("random_10"), 10);
        unique_ptr<SelectionOp> selection = make_unique<NTournamentSelection>(level, 5);
        unique_ptr<CrossoverOp> crossover = make_unique<OrderCrossover>(level);
        unique_ptr<MutationOp> mutation = make_unique<DisplacementMutation>(level, 0.05);
        unique_ptr<Replacement> replacement = make_unique<SteadyStateReplacement>(level, 20);

        auto time_limit = make_unique<TimeLimitCriterion>(level, 10);
        auto non_improving_gen = make_unique<MaxNonImprovingGenerationsCriterion>(level, 100);
        vector<unique_ptr<StoppingCriterion>> stopping;
        stopping.push_back(move(time_limit));
        stopping.push_back(move(non_improving_gen));

        GeneticAlgorithm ga(chromosomes, selection, crossover, mutation,
                            replacement, stopping);

        ga.start(100);
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
