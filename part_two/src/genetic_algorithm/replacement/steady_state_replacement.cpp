#include "genetic_algorithm/replacement/steady_state_replacement.h"

#include <utils/path.h>

SteadyStateReplacement::SteadyStateReplacement(
    const Logger::Level log_level, const size_t n_replaced_parents)
    : Replacement(log_level), n_replaced_parents(n_replaced_parents) {
}

vector<Chromosome> SteadyStateReplacement::replace(const vector<Chromosome> &parents,
                                                   const vector<Chromosome> &offsprings) const {
    auto parents_idx_by_fitness = sort_by_fitness_idx(parents, by_fitness_desc);
    auto offsprings_idx_by_fitness = sort_by_fitness_idx(offsprings, by_fitness_desc);

    vector<Chromosome> new_population;
    new_population.reserve(parents.size());
    const auto to_be_replaced = min(n_replaced_parents, offsprings.size());

    for (size_t i = 0; i < parents.size(); ++i) {
        if (i < to_be_replaced) {
            // Keep the remaining parents
            new_population.push_back(parents[parents_idx_by_fitness.top().first]);
            parents_idx_by_fitness.pop();
        } else {
            // Replace the worst parents with the best offsprings
            new_population.push_back(offsprings[offsprings_idx_by_fitness.top().first]);
            offsprings_idx_by_fitness.pop();
        }
    }

    return new_population;
}
