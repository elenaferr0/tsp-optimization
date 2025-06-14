#include "genetic_algorithm/selection/linear_ranking_selection.h"

#include <utils/path.h>

LinearRankingSelection::LinearRankingSelection(const Logger::Level log_level, const HyperParams &params)
    : SelectionOp(log_level, params) {
    log.set_label("LinearRankingSelection");
}

vector<Chromosome> LinearRankingSelection::select(const vector<Chromosome> &population) {
    vector<Chromosome> selected;
    selected.reserve(params.selection_n_parents);

    const int N = static_cast<int>(population.size());

    const auto sorted_population = sort_by_fitness_asc(population);
    vector<double> cumulative_probs(N);
    double cumulative_prob = 0.0;

    for (int i = 0; i < N; ++i) {
        const auto sigma_i = N - i;
        // Calculate selection probability according to linear ranking formula
        const auto p_i = 2.0 * sigma_i / (N * (N + 1));
        cumulative_prob += p_i;
        cumulative_probs[i] = cumulative_prob;
    }

    // Select individuals using roulette wheel
    for (int i = 0; i < params.selection_n_parents; ++i) {
        const double r = unif_real(0.0, 1.0);

        // Find the first individual whose cumulative probability exceeds r
        for (int j = 0; j < N; ++j) {
            if (r <= cumulative_probs[j]) {
                selected.push_back(sorted_population[j]);
                break;
            }
        }
    }

    log.trace("Selected " + std::to_string(selected.size()) + " chromosomes using linear ranking selection");
    if (log.get_min_level() <= Logger::Level::DEBUG) { // Avoid iterating if trace level is not set
        // Join fitnesses in a string for logging
        std::string fitnesses;
        for (const auto &chromosome: selected) {
            fitnesses += std::to_string(chromosome.evaluate_fitness()) + " ";
        }
        log.trace("Fitnesses: " + fitnesses);
    }

    return selected;
}
