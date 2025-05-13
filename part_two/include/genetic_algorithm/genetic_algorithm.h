#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H
#include <vector>

#include "chromosome/chromosome.h"
#include "crossover/crossover_op.h"
#include "mutation/mutation_op.h"
#include "replacement/generational_replacement.h"
#include "stopping/stopping_criteria.h"
#include "selection/selection_op.h"

class GeneticAlgorithm {
    std::vector<Chromosome> population;
    SelectionOp selection;
    CrossoverOp crossover;
    MutationOp mutation;
    GenerationalReplacement replacement;
    StoppingCriteria stopping;

    void evaluate_fitness();

public:
    GeneticAlgorithm(const std::vector<Chromosome> &initial_population,
                     const SelectionOp &selection,
                     const CrossoverOp &crossover,
                     const MutationOp &mutation,
                     const GenerationalReplacement &replacement,
                     const StoppingCriteria &stopping);
};

#endif //GENETIC_ALGORITHM_H
