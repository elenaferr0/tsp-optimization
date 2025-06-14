#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H
#include <utils/logger.h>
#include <vector>
#include <genetic_algorithm/hyper_params.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class MutationOp {
protected:
  Logger log;
  HyperParams params;

public:
  explicit MutationOp(Logger::Level log_level, const HyperParams &params);
  virtual ~MutationOp() = default;

  virtual vector<Chromosome>
  mutate(const vector<Chromosome> &population) const = 0;
};

#endif // MUTATION_OPERATOR_H
