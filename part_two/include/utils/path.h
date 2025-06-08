#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include <algorithm>
#include <iostream>
#include "utils/typedefs.h"
#include "utils/maths.h"

// Generates two random cut points for a path of length n
inline priority_queue_asc<int> random_cut_points(const size_t parents_len) {
    if (parents_len < 2) {
        throw std::invalid_argument("Path length must be at least 2 to generate cut points.");
    }

    priority_queue_asc<int> cuts;
    constexpr int n_cuts = 2;

    while (cuts.size() < n_cuts) {
        int cut = unif(0, parents_len - 1);
        if (cuts.empty() || cuts.top() != cut) {
            cuts.push(cut);
        }
    }

    return cuts;
}

// Returns a priority queue of (index, fitness) pairs sorted according to the provided comparator.
inline priority_queue_by_fitness sort_by_fitness_idx(
    const vector<Chromosome> &chromosomes,
    const pair_comp &comparator
) {
    priority_queue_by_fitness pq(comparator);
    for (size_t i = 0; i < chromosomes.size(); ++i) {
        pq.emplace(i, chromosomes[i].evaluate_fitness());
    }
    return pq;
}

// Sorts the chromosomes by fitness using the provided comparator and returns a sorted vector.
inline vector<Chromosome> sort_by_fitness(
    const vector<Chromosome> &chromosomes // Use operator<
) {
    vector<Chromosome> sorted_chromosomes = chromosomes;
    std::sort(sorted_chromosomes.begin(), sorted_chromosomes.end());
    return sorted_chromosomes;
}

#endif //PATH_UTILS_H
