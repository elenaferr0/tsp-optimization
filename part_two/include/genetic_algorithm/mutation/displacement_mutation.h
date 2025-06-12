#ifndef DISPLACEMENT_MUTATION_H
#define DISPLACEMENT_MUTATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/mutation/mutation_op.h"

class DisplacementMutation final : public MutationOp {
public:
  explicit DisplacementMutation(Logger::Level log_level, double rate);

  vector<Chromosome> mutate(const vector<Chromosome> &population) const override;
};

#endif // DISPLACEMENT_MUTATION_H
