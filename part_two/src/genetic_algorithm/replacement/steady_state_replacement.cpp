#include "genetic_algorithm/replacement/steady_state_replacement.h"

#include <utils/path.h>

SteadyStateReplacement::SteadyStateReplacement(const Logger::Level log_level,
                                               const double worst_replacement_ratio)
    : Replacement(log_level), worst_replacement_ratio(worst_replacement_ratio) {
}

vector<Chromosome> SteadyStateReplacement::replace(const vector<Chromosome> &parents,
                                                   const vector<Chromosome> &offsprings) {
    const auto parents_by_fitness = sort_by_fitness_asc(parents);
    const auto offsprings_by_fitness = sort_by_fitness_asc(offsprings);

    vector<Chromosome> new_population;
    new_population.reserve(parents.size());
    const int n_replaced_parents = floor(worst_replacement_ratio * static_cast<int>(parents.size()));
    const auto to_be_replaced = min(n_replaced_parents, static_cast<int>(offsprings_by_fitness.size()));

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
