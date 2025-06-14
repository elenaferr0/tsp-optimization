#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

#include <stdexcept>

struct HyperParams {
  int population_size = 100;
  double mutation_rate = 0.01;
  double parents_replacement_rate = 0.8;
  int selection_n_parents = 2;
  int selection_tournament_size = 3;
  long time_limit_seconds = 60;
  long max_non_improving_generations = 100;
  // How far (in terms of node w.r.t. the population size) the convex hull initialization can insert internal nodes
  // instead of the best position in the subtour.
  double convex_hull_max_deviation = 0.1;
  // How frequently the convex hull initialization can deviate from the best position
  double convex_hull_deviation_ratio = 0.1;

  void validate_or_throw() const {
    if (population_size <= 0) {
      throw std::invalid_argument("Population size must be a positive integer.");
    }
    if (mutation_rate < 0.0 || mutation_rate > 1.0) {
      throw std::invalid_argument("Mutation rate must be in the range [0.0, 1.0].");
    }

    if (parents_replacement_rate < 0.0 || parents_replacement_rate > 1.0) {
      throw std::invalid_argument("Replacement rate must be in the range [0.0, 1.0].");
    }

    if (selection_n_parents <= 0) {
      throw std::invalid_argument("Number of parents for selection must be a positive integer.");
    }

    if (selection_tournament_size <= 0) {
      throw std::invalid_argument("Tournament size for selection must be a positive integer.");
    }

    if (time_limit_seconds < 0) {
      throw std::invalid_argument("Time limit must be a non-negative integer.");
    }

    if (max_non_improving_generations < 0) {
      throw std::invalid_argument("Max non-improving generations must be a non-negative integer.");
    }

    if (convex_hull_max_deviation < 0.0 || convex_hull_max_deviation > 1.0) {
      throw std::invalid_argument("Convex hull max deviation must be in the range [0.0, 1.0].");
    }

    if (convex_hull_deviation_ratio < 0.0 || convex_hull_deviation_ratio > 1.0) {
      throw std::invalid_argument("Convex hull deviation ratio must be in the range [0.0, 1.0].");
    }
  }
};

#endif //HYPER_PARAMS_H
