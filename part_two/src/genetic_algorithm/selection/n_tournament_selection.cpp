#include "genetic_algorithm/selection/n_tournament_selection.h"

#include <algorithm>
#include <float.h>
#include <set>

#include <iostream>
#include <random>

using namespace std;

NTournamentSelection::NTournamentSelection(int tournament_size) : SelectionOp(), tournament_size(tournament_size) {
}

vector<ChromosomePtr> NTournamentSelection::select(const vector<ChromosomePtr> &population) {
    int size = min(tournament_size, static_cast<int>(population.size()));

    random_device rd;
    mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, size - 1);

    vector<ChromosomePtr> selected;
    selected.reserve(n_parents);

    for (size_t i = 0; i < n_parents; ++i) {
        multiset<ChromosomePtr> tournament; // sorted by operator< (fitness)

        while (tournament.size() < tournament_size) {
            size_t random_index = dist(gen);
            tournament.insert(population[random_index]);
        }

        selected.push_back(*tournament.begin()); // select the best
    }

    return selected;
}
