#include "genetic_algorithm/replacement/steady_state_replacement.h"

#include <utils/path.h>

SteadyStateReplacement::SteadyStateReplacement(const Logger::Level log_level,
                                               const size_t n_replaced_parents)
    : Replacement(log_level), n_replaced_parents(n_replaced_parents) {
}

vector<Chromosome> SteadyStateReplacement::replace(const vector<Chromosome> &parents,
                                                   const vector<Chromosome> &offsprings) {
    const auto parents_by_fitness = sort_by_fitness(parents);
    const auto offsprings_by_fitness = sort_by_fitness(offsprings);

    vector<Chromosome> new_population;
    new_population.reserve(parents.size());
    const auto to_be_replaced = min(n_replaced_parents, offsprings.size());

    for (size_t i = 0; i < parents.size(); ++i) {
        if (i < to_be_replaced) {
            // Replace the worst parents with the best offsprings
            new_population.emplace_back(offsprings_by_fitness[i]);
        } else {
            // Keep the remaining parents
            new_population.emplace_back(parents_by_fitness[i - to_be_replaced]);
        }
    }

    return new_population;
}
