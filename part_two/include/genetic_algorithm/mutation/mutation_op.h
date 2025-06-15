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

public:
  explicit MutationOp(Logger::Level log_level);
  virtual ~MutationOp() = default;

  virtual vector<Chromosome> mutate(const HyperParams& params, const vector<Chromosome> &population) = 0;
};

#endif // MUTATION_OPERATOR_H
