#ifndef STEADY_STATE_REPLACEMENT_H
#define STEADY_STATE_REPLACEMENT_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "replacement.h"

using namespace std;

class SteadyStateReplacement final : public Replacement {
public:
    explicit SteadyStateReplacement(const Logger::Level log_level);

    string name() const override;

    vector<Chromosome> replace(const HyperParams &params, const vector<Chromosome> &parents,
                               const vector<Chromosome> &offsprings) const override;
};

#endif // STEADY_STATE_REPLACEMENT_H
