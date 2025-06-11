#include "genetic_algorithm/selection/n_tournament_selection.h"

#include <algorithm>
#include <set>

#include <iostream>
#include <random>

using namespace std;

NTournamentSelection::NTournamentSelection(const Logger::Level log_level,
                                           const int tournament_size)
    : SelectionOp(log_level), tournament_size(tournament_size) {
  log.set_label("NTournamentSelection");
}

vector<Chromosome>
NTournamentSelection::select(const vector<Chromosome> &population) {
  const int size = min(tournament_size, static_cast<int>(population.size()));

  random_device rd;
  mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, size - 1);

  vector<Chromosome> selected;
  selected.reserve(n_parents);

  for (size_t i = 0; i < n_parents; ++i) {
    multiset<Chromosome> tournament; // sorted by operator< (fitness)

    while (tournament.size() < tournament_size) {
      const size_t random_index = dist(gen);
      tournament.insert(population[random_index]);
    }

    selected.push_back(*tournament.begin()); // select the best
  }

  if (log.get_min_level() <
      Logger::Level::INFO) { // Avoid iterating if log level is too low
    log.debug("Selected " + std::to_string(selected.size()) +
              " chromosomes using n-tournament selection:");
    for (const auto &chromosome : selected) {
      log.debug(chromosome.to_str());
    }
  }

  return selected;
}
