#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include <memory>
#include <vector>

#include "chromosome/chromosome.h"
#include "crossover/crossover_op.h"
#include "initialization/population_initialization.h"
#include "mutation/mutation_op.h"
#include "replacement/replacement.h"
#include "selection/selection_op.h"
#include "stopping/stopping_criterion.h"
#include "utils/logger.h"

using namespace std;

class GeneticAlgorithm {
    vector<Chromosome> population;
    vector<unique_ptr<PopulationInitialization>> population_init;
    unique_ptr<SelectionOp> selection;
    unique_ptr<CrossoverOp> crossover;
    unique_ptr<MutationOp> mutation;
    unique_ptr<Replacement> replacement;
    vector<unique_ptr<StoppingCriterion>> stopping;
    long generation_n;
    Logger log;
    double initial_fitness;

    Chromosome get_best() const;

    bool should_stop();

    void handle_start();

    void print_summary();

    void save_to_file(const string &filename);

public:
    GeneticAlgorithm(vector<unique_ptr<PopulationInitialization>> &population_init,
                     unique_ptr<SelectionOp> &selection,
                     unique_ptr<CrossoverOp> &crossover,
                     unique_ptr<MutationOp> &mutation,
                     unique_ptr<Replacement> &replacement,
                        vector<unique_ptr<StoppingCriterion>> &stopping,
                     Logger::Level log_level = Logger::Level::INFO);

    void start(const string &filename, long logging_frequency = 100);
};

#endif // GENETIC_ALGORITHM_H
