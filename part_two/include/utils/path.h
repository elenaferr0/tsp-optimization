#ifndef PATH_UTILS_H
#define PATH_UTILS_H

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

inline priority_queue_by_fitness sort_by_fitness_desc(
    const vector<Chromosome> &chromosomes) {
    priority_queue_by_fitness pq(by_fitness_desc);
    for (size_t i = 0; i < chromosomes.size(); ++i) {
        pq.emplace(i, chromosomes[i].evaluate_fitness());
    }
    return pq;
}

#endif //PATH_UTILS_H
