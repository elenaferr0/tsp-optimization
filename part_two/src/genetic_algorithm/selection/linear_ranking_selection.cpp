#include "genetic_algorithm/selection/linear_ranking_selection.h"

#include <utils/path.h>

LinearRankingSelection::LinearRankingSelection(const Logger::Level log_level,
                                               const int n_parents)
    : SelectionOp(log_level, n_parents) {
  if (n_parents <= 0) {
    throw std::invalid_argument("n_parents must be greater than 0");
  }
  log.set_label("LinearRankingSelection");
}

vector<Chromosome>
LinearRankingSelection::select(const vector<Chromosome> &population) {
  vector<Chromosome> selected;
  selected.reserve(n_parents);

  const int N = static_cast<int>(population.size());

  const auto sorted_population = sort_by_fitness(population);
  vector<double> cumulative_probs(N);
  double cumulative_prob = 0.0;

  for (int i = 0; i < N; ++i) {
    const auto sigma_i = i + 1;
    // Calculate selection probability according to linear ranking formula
    const auto p_i = 2.0 * sigma_i / (N * (N + 1));
    cumulative_prob += p_i;
    cumulative_probs[i] = cumulative_prob;
  }

  // Select individuals using roulette wheel
  for (int i = 0; i < n_parents; ++i) {
    const double r = unif(0.0, 1.0);

    // Find the first individual whose cumulative probability exceeds r
    for (int j = 0; j < N; ++j) {
      if (r <= cumulative_probs[j]) {
        selected.push_back(sorted_population[j]);
        break;
      }
    }
  }

  if (log.get_min_level() <
      Logger::Level::INFO) { // Avoid iterating if log level is too low
    log.debug("Selected " + std::to_string(selected.size()) +
              " chromosomes using linear ranking selection:");
    for (const auto &chromosome : selected) {
      log.debug(chromosome.to_str());
    }
  }

  return selected;
}
