#include "genetic_algorithm/genetic_algorithm.h"

GeneticAlgorithm::GeneticAlgorithm(
    const vector<ChromosomePtr> &initial_population,
    unique_ptr<SelectionOp> &selection,
    unique_ptr<CrossoverOp> &crossover,
    unique_ptr<MutationOp> &mutation,
    unique_ptr<GenerationalReplacement> &replacement,
    unique_ptr<StoppingCriteria> &stopping
) : population(initial_population),
    selection(move(selection)),
    crossover(move(crossover)),
    mutation(move(mutation)),
    replacement(move(replacement)),
    stopping(move(stopping)) {
}

void GeneticAlgorithm::start() {
}
