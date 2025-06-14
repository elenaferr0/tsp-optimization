#include <iostream>
#include <memory>

#include "tsp/graph.h"
#include <utils/path.h>
#include <cassert>

#include "genetic_algorithm/genetic_algorithm.h"
#include "genetic_algorithm/initialization/random_initialization.h"
#include "genetic_algorithm/initialization/convex_hull_initialization.h"
#include "genetic_algorithm/selection/linear_ranking_selection.h"
#include "genetic_algorithm/crossover/order_crossover.h"
#include "genetic_algorithm/replacement/elitism_replacement.h"
#include "genetic_algorithm/replacement/steady_state_replacement.h"
#include "genetic_algorithm/selection/n_tournament_selection.h"
#include "genetic_algorithm/stopping/max_non_improving_generations_criterion.h"
#include "genetic_algorithm/stopping/time_limit_criterion.h"
#include "../include/genetic_algorithm/mutation/simple_inversion_mutation.h"

using namespace std;

void parse_params(const int argc, char *argv[], char *&instance)
{
    if (argc < 1)
    {
        std::cerr << "Usage: " << argv[0] << " <instance_name>" << std::endl;
        exit(1);
    }

    instance = argv[1];
    if (instance == nullptr || instance[0] == '\0')
    {
        throw std::invalid_argument("Instance name must be provided as the first argument.");
        exit(1);
    }
}

int main(const int argc, char *argv[])
{
    // char *instance = nullptr;
    // parse_params(argc, argv, instance);

    auto instance = "bayg29";
    // auto instance = "random_10";

    try
    {
        auto level = Logger::Level::DEBUG;

        int population_size = 500;

        // unique_ptr<PopulationInitialization> chromosomes = make_unique<ConvexHullInitialization>(level, Graph::from_file(instance), population_size);
        unique_ptr<PopulationInitialization> chromosomes = make_unique<RandomInitialization>(level, Graph::from_file(instance), population_size);
        // unique_ptr<SelectionOp> selection = make_unique<NTournamentSelection>(level, 10, population_size * 3 / 4);
        unique_ptr<SelectionOp> selection = make_unique<LinearRankingSelection>(level, population_size / 2);
        unique_ptr<CrossoverOp> crossover = make_unique<OrderCrossover>(level);
        unique_ptr<MutationOp> mutation = make_unique<SimpleInversionMutation>(level, 0.5);
        unique_ptr<Replacement> replacement = make_unique<SteadyStateReplacement>(level, 0.2);

        auto time_limit = make_unique<TimeLimitCriterion>(level, 10);
        auto non_improving_gen = make_unique<MaxNonImprovingGenerationsCriterion>(level, 100);
        vector<unique_ptr<StoppingCriterion>> stopping;
        // stopping.push_back(move(time_limit));
        stopping.push_back(std::move(non_improving_gen));

        GeneticAlgorithm ga(chromosomes, selection, crossover, mutation,
                            replacement, stopping);

        ga.start(instance, 5);
    }
    catch (std::exception &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
