#include "genetic_algorithm/genetic_algorithm.h"

#include <algorithm>
#include <cassert>

using namespace chrono;

GeneticAlgorithm::GeneticAlgorithm(const vector<shared_ptr<PopulationInitialization> > &population_init,
                                   unique_ptr<SelectionOp> selection,
                                   unique_ptr<CrossoverOp> crossover,
                                   unique_ptr<MutationOp> mutation,
                                   unique_ptr<Replacement> replacement,
                                   const vector<shared_ptr<StoppingCriterion> > &stopping,
                                   const Logger::Level log_level)
    : population_init(population_init), selection(std::move(selection)),
      crossover(std::move(crossover)), mutation(std::move(mutation)),
      replacement(std::move(replacement)), stopping(stopping), generation_n(0),
      log(log_level, "GeneticAlgorithm"),
      initial_fitness(numeric_limits<double>::max()) {
}

bool GeneticAlgorithm::should_stop(const HyperParams &params) const {
    for (const auto &criterion: stopping) {
        if (criterion->should_stop(params)) {
            return true;
        }
    }

    return false;
}

void GeneticAlgorithm::handle_start(const HyperParams &params) const {
    for (const auto &criterion: stopping) {
        criterion->handle_start(params);
    }
}

Chromosome GeneticAlgorithm::start(const HyperParams &params, const long logging_frequency) {
    log.debug("Initializing population");
    const high_resolution_clock::time_point start_time = high_resolution_clock::now();
    for (const auto &init: population_init) {
        auto init_population = init->generate_population(params);
        population.insert(population.end(), init_population.begin(), init_population.end());
    }

    if (population.empty()) {
        throw runtime_error("Initial population cannot be empty");
    }

    auto best_chromosome = get_best();
    initial_fitness = best_chromosome.evaluate_fitness();
    log.debug("Initial best chromosome fitness: " + to_string(initial_fitness));

    log.debug("Starting genetic algorithm with population size: " + to_string(population.size()));

    handle_start(params);
    while (not should_stop(params)) {
        log.debug("Starting new generation_n");
        vector<Chromosome> parents = selection->select(params, population);
        log.trace("Selected " + to_string(parents.size()) + " parents for crossover");

        vector<Chromosome> offsprings = crossover->recombine(params, parents);
        log.trace("Generated " + to_string(offsprings.size()) + " offspring from crossover");

        vector<Chromosome> mutated = mutation->mutate(params, offsprings);
        log.trace("Applied mutation, resulting in " + to_string(mutated.size()) + " mutated offspring");

        population = replacement->replace(params, parents, offsprings);
        log.trace("Replaced generation_n");

        generation_n++;
        // auto old_fitness = best_chromosome.evaluate_fitness();
        best_chromosome = get_best();
        // if (best_chromosome.evaluate_fitness() > old_fitness)
        // {
        //     throw runtime_error("Best chromosome fitness increased from " + to_string(old_fitness) +
        //                          " to " + to_string(best_chromosome.evaluate_fitness()));
        // }
        if (generation_n % logging_frequency == 0) {
            log.debug("Generation " + to_string(generation_n) + " best chromosome fitness: " +
                      to_string(best_chromosome.evaluate_fitness()));
        }
    }

    log.info("Final fitness of best chromosome: " + to_string(best_chromosome.evaluate_fitness()));

    const auto end_time = high_resolution_clock::now();
    const auto duration = chrono::duration_cast<milliseconds>(end_time - start_time).count();
    log.info("Genetic algorithm completed in " + to_string(duration) + " ms");
    return best_chromosome;
}

Chromosome GeneticAlgorithm::get_best() const {
    if (population.empty()) {
        throw runtime_error("Population is empty, cannot find best chromosome");
    }
    const auto best_it = min_element(population.begin(), population.end(),
                                     [](const Chromosome &a, const Chromosome &b) {
                                         return a.evaluate_fitness() < b.evaluate_fitness();
                                     });

    if (best_it->evaluate_fitness() < 0) {
        throw runtime_error("Best chromosome fitness is negative");
    }
    return *best_it;
}
