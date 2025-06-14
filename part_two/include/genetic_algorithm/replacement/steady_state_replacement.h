#ifndef STEADY_STATE_REPLACEMENT_H
#define STEADY_STATE_REPLACEMENT_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "replacement.h"

using namespace std;

class SteadyStateReplacement final : public Replacement {
  double worst_replacement_ratio;
public:
  explicit SteadyStateReplacement(Logger::Level log_level, double worst_replacement_ratio = 0.1);

  vector<Chromosome>
  replace(const vector<Chromosome> &parents,
          const vector<Chromosome> &offsprings) override;
};

#endif // STEADY_STATE_REPLACEMENT_H
