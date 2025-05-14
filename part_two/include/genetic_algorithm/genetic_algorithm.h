#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include <memory>
#include <vector>

#include "chromosome/chromosome.h"
#include "crossover/crossover_op.h"
#include "mutation/mutation_op.h"
#include "replacement/generational_replacement.h"
#include "stopping/stopping_criteria.h"
#include "selection/selection_op.h"

using namespace std;

class GeneticAlgorithm {
    vector<ChromosomePtr> population;
    unique_ptr<SelectionOp> selection;
    unique_ptr<CrossoverOp> crossover;
    unique_ptr<MutationOp> mutation;
    unique_ptr<GenerationalReplacement> replacement;
    unique_ptr<StoppingCriteria> stopping;

public:
    GeneticAlgorithm(const vector<ChromosomePtr> &initial_population,
                     unique_ptr<SelectionOp> &selection,
                     unique_ptr<CrossoverOp> &crossover,
                     unique_ptr<MutationOp> &mutation,
                     unique_ptr<GenerationalReplacement> &replacement,
                     unique_ptr<StoppingCriteria> &stopping);

    void start();
};

#endif //GENETIC_ALGORITHM_H
