#ifndef STEADY_STATE_REPLACEMENT_H
#define STEADY_STATE_REPLACEMENT_H

#include "replacement.h"
#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SteadyStateReplacement final : public Replacement {
  public:
    SteadyStateReplacement();
    vector<Chromosome> replace(const vector<Chromosome> &parents,
                                          const vector<Chromosome> &offsprings) const override;
};

#endif //STEADY_STATE_REPLACEMENT_H
