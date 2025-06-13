#ifndef SIMPLE_INVERSION_MUTATION_H
#define SIMPLE_INVERSION_MUTATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/mutation/mutation_op.h"

class SimpleInversionMutation final : public MutationOp {
public:
  explicit SimpleInversionMutation(Logger::Level log_level, double rate);

  vector<Chromosome> mutate(const vector<Chromosome> &population) const override;
};

#endif // SIMPLE_INVERSION_MUTATION_H
