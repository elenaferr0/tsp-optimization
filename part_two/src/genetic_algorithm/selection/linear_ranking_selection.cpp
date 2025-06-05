#include "genetic_algorithm/selection/linear_ranking_selection.h"

#include <algorithm>
#include <map>

LinearRankingSelection::LinearRankingSelection(const int n_parents) : SelectionOp(n_parents) {
}

vector<Chromosome> LinearRankingSelection::select(const vector<Chromosome> &population) {
    // Create sorted copy of the population based on fitness
    auto sorted = population;
    sort(
        sorted.begin(),
        sorted.end(),
        [](const auto &a, const auto &b) { return a.evaluate_fitness() < b.evaluate_fitness(); }
    );

    // TODO: not very elegant and efficient, find a better way
    map<double, Chromosome> selected;
    for (size_t i = 0; i < sorted.size(); ++i) {
        const auto sigma_i = i + 1;
        const auto p_i = 2 * sigma_i / (sorted.size() * (sorted.size() + 1));
        selected[p_i] = sorted[i];
    }

    vector<Chromosome> top_n;
    for (int i = 0; i < n_parents; ++i) {
        top_n.push_back(sorted[i]);
    }
    return top_n;
}
