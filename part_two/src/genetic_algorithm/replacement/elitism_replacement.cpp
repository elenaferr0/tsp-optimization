#include "genetic_algorithm/replacement/elitism_replacement.h"
#include "utils/path.h"

ElitismReplacement::ElitismReplacement(const Logger::Level log_level)
    : Replacement(Logger(log_level, name())) {
}

vector<Chromosome> ElitismReplacement::replace(const HyperParams &params, const vector<Chromosome> &parents,
                                               const vector<Chromosome> &offsprings) const {
    if (parents.empty() || offsprings.empty()) {
        throw std::invalid_argument("ElitismReplacement: parents and offsprings cannot be empty");
    }

    const int replacement_n = floor(params.parents_replacement_rate * static_cast<int>(parents.size()));
    auto num_elites = min(replacement_n, static_cast<int>(offsprings.size()));
    if (num_elites == 0) {
        num_elites = 1; // Ensure at least one elite is selected
    }

    log.trace("Selecting " + to_string(num_elites) + " elites from parents");

    vector<Chromosome> sorted_parents = sort_by_fitness_asc(parents);
    vector<Chromosome> sorted_offsprings = sort_by_fitness_asc(offsprings);

    vector<Chromosome> new_population;
    new_population.reserve(parents.size());

    for (size_t i = 0; i < parents.size(); ++i) {
        if (i < num_elites) {
            new_population.emplace_back(sorted_parents[i]);
        } else {
            new_population.emplace_back(sorted_offsprings[i - num_elites]);
        }
    }

    // Ensure the new population size matches the original parents size
    if (new_population.size() != parents.size()) {
        throw std::runtime_error("ElitismReplacement: new population size does not match parents size");
    }
    log.trace("Replaced " + to_string(num_elites) + " elites from parents with offsprings");
    return new_population;
}

unique_ptr<Replacement> ElitismReplacement::clone() const {
    return make_unique<ElitismReplacement>(*this);
}

string ElitismReplacement::name() const {
    return "ElitismReplacement";
}
