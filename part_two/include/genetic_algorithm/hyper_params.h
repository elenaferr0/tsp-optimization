#ifndef HYPER_PARAMS_H
#define HYPER_PARAMS_H

#include <stdexcept>

using namespace std;

struct HyperParams {
  int population_size = 500;
  double mutation_rate = 0.01;
  double parents_replacement_rate = 0.8;
  int selection_n_parents = 2;
  int selection_tournament_size = 3;
  long time_limit_seconds = 60;
  long max_non_improving_generations = 100;
  pair<double, double> convex_hull_random_init_ratio = {0.1, 0.9}; // Convex hull and random initialization percentages

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

    if (convex_hull_random_init_ratio.first < 0.0 || convex_hull_random_init_ratio.first > 1.0 ||
        convex_hull_random_init_ratio.second < 0.0 || convex_hull_random_init_ratio.second > 1.0 ||
        convex_hull_random_init_ratio.first + convex_hull_random_init_ratio.second != 1.0) {
      throw std::invalid_argument("Initialization percentages must be in the range [0.0, 1.0] and sum to 1.0.");
    }
  }
};

#endif //HYPER_PARAMS_H
