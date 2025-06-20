#ifndef ELITISM_REPLACEMENT_H
#define ELITISM_REPLACEMENT_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "replacement.h"

using namespace std;

class ElitismReplacement final : public Replacement {
public:
    explicit ElitismReplacement(Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> replace(const HyperParams &params, const vector<Chromosome> &parents,
                               const vector<Chromosome> &offsprings) const override;
};

#endif // ELITISM_REPLACEMENT_H
