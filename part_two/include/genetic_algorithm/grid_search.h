#ifndef GRID_SEARCH_H
#define GRID_SEARCH_H

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
#include "genetic_algorithm/grid_search.h"
#include <vector>

using namespace std;

class GridSearch {
    string instance_name;
    vector<double> mutation_rates;
    vector<double> parent_replacement_rates;
    vector<int> selection_tournament_sizes;
    vector<pair<double, double> > convex_hull_random_init_ratios;
    const Logger log;

    pair<Chromosome, HyperParams> run_experiment(
        const vector<shared_ptr<PopulationInitialization>> &initializations,
        const unique_ptr<SelectionOp> &selection,
        const unique_ptr<CrossoverOp> &crossover,
        const unique_ptr<MutationOp> &mutation,
        const unique_ptr<Replacement> &replacement,
        const vector<shared_ptr<StoppingCriterion>> &stopping,
        double mutation_rate,
        double parent_replacement_rate,
        int selection_tournament_size,
        double convex_hull_init_ratio,
        double random_init_ratio
    ) const;

public:
    GridSearch(
        const string &instance_name,
        const vector<double> &mutation_rates,
        const vector<double> &parent_replacement_rates,
        const vector<int> &selection_tournament_sizes,
        const vector<pair<double, double> > &convex_hull_random_init_ratios
    );

    void run() const;
};

#endif //GRID_SEARCH_H
