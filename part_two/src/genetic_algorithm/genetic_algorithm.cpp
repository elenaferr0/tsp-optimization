#include "genetic_algorithm/genetic_algorithm.h"

#include <algorithm>

GeneticAlgorithm::GeneticAlgorithm(unique_ptr<PopulationInitialization> &population_initialization,
                                   unique_ptr<SelectionOp> &selection,
                                   unique_ptr<CrossoverOp> &crossover,
                                   unique_ptr<MutationOp> &mutation,
                                   unique_ptr<Replacement> &replacement,
                                   vector<unique_ptr<StoppingCriterion> > &stopping,
                                   const Logger::Level log_level)
    : population_initialization(move(population_initialization)), selection(move(selection)),
      crossover(move(crossover)), mutation(move(mutation)),
      replacement(move(replacement)), stopping(move(stopping)), generation(0),
      log(log_level, "GeneticAlgorithm") {
}

bool GeneticAlgorithm::should_stop() {
    for (const auto &criterion: stopping) {
        if (criterion->should_stop()) {
            return true;
        }
    }

    return false;
}


void GeneticAlgorithm::handle_start() {
    for (const auto &criterion: stopping) {
        criterion->handle_start();
    }
}

void GeneticAlgorithm::start(const long logging_frequency) {
    log.info("Initializing population");
    population = population_initialization->generate_population();
    if (population.empty()) {
        throw runtime_error("Initial population cannot be empty");
    }

    auto best_chromosome = get_best();
    log.info("Initial best chromosome fitness: " + to_string(best_chromosome.evaluate_fitness()));

    log.info("Starting genetic algorithm with population size: " +
             to_string(population.size()));

    handle_start();
    while (not should_stop()) {
        log.debug("Starting new generation");
        vector<Chromosome> parents = selection->select(population);
        log.trace("Selected " + to_string(parents.size()) + " parents for crossover");

        vector<Chromosome> offspring = crossover->recombine(parents);
        log.trace("Generated " + to_string(offspring.size()) + " offspring from crossover");

        vector<Chromosome> mutated = mutation->mutate(offspring);
        log.trace("Applied mutation, resulting in " + to_string(mutated.size()) + " mutated offspring");

        population = replacement->replace(population, offspring);
        log.trace("Replaced generation");

        generation++;
        best_chromosome = get_best();
        if (generation % logging_frequency == 0) {
            log.info("Generation " + to_string(generation) + " best chromosome fitness: " +
                     to_string(best_chromosome.evaluate_fitness()));
        }
    }
    log.info("Genetic algorithm completed in " + to_string(generation) + " generations with best fitness: " +
             to_string(best_chromosome.evaluate_fitness()));
}

Chromosome GeneticAlgorithm::get_best() const {
    if (population.empty()) {
        throw runtime_error("Population is empty, cannot find best chromosome");
    }
    const auto best_it = min_element(population.begin(), population.end(),
                                     [](const Chromosome &a, const Chromosome &b) {
                                         return a.evaluate_fitness() < b.evaluate_fitness();
                                     });
    return *best_it;
}
