#ifndef STEADY_STATE_REPLACEMENT_H
#define STEADY_STATE_REPLACEMENT_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "replacement.h"

using namespace std;

class SteadyStateReplacement final : public Replacement {
public:
  explicit SteadyStateReplacement(Logger::Level log_level, const HyperParams &params);

  vector<Chromosome>
  replace(const vector<Chromosome> &parents,
          const vector<Chromosome> &offsprings) override;
};

#endif // STEADY_STATE_REPLACEMENT_H
