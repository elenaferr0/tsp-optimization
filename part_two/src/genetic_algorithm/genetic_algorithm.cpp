#include "genetic_algorithm/genetic_algorithm.h"

GeneticAlgorithm::GeneticAlgorithm(const vector<ChromosomePtr> &initial_population,
                                   const SelectionOp &selection,
                                   const CrossoverOp &crossover,
                                   const MutationOp &mutation,
                                   const GenerationalReplacement &replacement,
                                   const StoppingCriteria &stopping
) : population(initial_population),
    selection(make_unique<SelectionOp>(selection)),
    crossover(make_unique<CrossoverOp>(crossover)),
    mutation(make_unique<MutationOp>(mutation)),
    replacement(make_unique<GenerationalReplacement>(replacement)),
    stopping(make_unique<StoppingCriteria>(stopping)) {
}

void GeneticAlgorithm::start() {
}
