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
    vector<shared_ptr<PopulationInitialization> > population_init;
    unique_ptr<SelectionOp> selection;
    unique_ptr<CrossoverOp> crossover;
    unique_ptr<MutationOp> mutation;
    unique_ptr<Replacement> replacement;
    vector<shared_ptr<StoppingCriterion> > stopping;
    long generation_n;
    const Logger log;
    double initial_fitness;

    [[nodiscard]] Chromosome get_best() const;

    [[nodiscard]] bool should_stop(const HyperParams &params) const;

    void handle_start(const HyperParams &params) const;

public:
    GeneticAlgorithm(const vector<shared_ptr<PopulationInitialization> > &population_init,
                     unique_ptr<SelectionOp> selection,
                     unique_ptr<CrossoverOp> crossover,
                     unique_ptr<MutationOp> mutation,
                     unique_ptr<Replacement> replacement,
                     const vector<shared_ptr<StoppingCriterion> > &stopping,
                     Logger::Level log_level);

    Chromosome start(const HyperParams &params, long logging_frequency = 100);
};

#endif // GENETIC_ALGORITHM_H
