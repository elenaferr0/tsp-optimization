#include "genetic_algorithm/selection/n_tournament_selection.h"

#include <algorithm>
#include <set>

#include <iostream>
#include <random>

#include "utils/maths.h"

using namespace std;

NTournamentSelection::NTournamentSelection(const Logger::Level log_level, const int tournament_size, const int n_parents)
    : SelectionOp(log_level, n_parents), tournament_size(tournament_size) {
    log.set_label("NTournamentSelection");
}

vector<Chromosome> NTournamentSelection::select(const vector<Chromosome> &population) {
    vector<Chromosome> selected;
    selected.reserve(n_parents);

    for (size_t i = 0; i < n_parents; ++i) {
        multiset<Chromosome> tournament; // sorted by operator< (fitness)

        while (tournament.size() < tournament_size) {
            const size_t random_index = unif_int(0, population.size() - 1);
            tournament.insert(population[random_index]);
        }

        selected.push_back(*tournament.begin()); // select the best
    }

    log.trace("Selected " + std::to_string(selected.size()) + " chromosomes using n-tournament selection:");
    if (log.get_min_level() <= Logger::Level::DEBUG) {
        string fitnesses;
        for (const auto &chromosome: selected) {
            fitnesses += std::to_string(chromosome.evaluate_fitness()) + " ";
        }
        log.trace("Fitnesses: " + fitnesses);
    }

    return selected;
}
