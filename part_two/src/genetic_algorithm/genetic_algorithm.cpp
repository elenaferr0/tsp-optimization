#include "genetic_algorithm/genetic_algorithm.h"

GeneticAlgorithm::GeneticAlgorithm(const std::vector<Chromosome> &initial_population,
                                   const SelectionOp &selection,
                                   const CrossoverOp &crossover,
                                   const MutationOp &mutation,
                                   const GenerationalReplacement &replacement,
                                   const StoppingCriteria &stopping
) : population(initial_population),
    crossover(crossover),
    mutation(mutation),
    replacement(replacement),
    stopping(stopping),
    selection(selection) {
}
