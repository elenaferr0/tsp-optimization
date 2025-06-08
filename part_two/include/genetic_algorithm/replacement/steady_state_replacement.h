#ifndef STEADY_STATE_REPLACEMENT_H
#define STEADY_STATE_REPLACEMENT_H

#include "replacement.h"
#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SteadyStateReplacement final : public Replacement {
    size_t n_replaced_parents; // Number of parents to be replaced
public:
    explicit SteadyStateReplacement(Logger::Level log_level, size_t n_replaced_parents = 2);

    vector<Chromosome> replace(const vector<Chromosome> &parents,
                               const vector<Chromosome> &offsprings) const override;
};

#endif //STEADY_STATE_REPLACEMENT_H
