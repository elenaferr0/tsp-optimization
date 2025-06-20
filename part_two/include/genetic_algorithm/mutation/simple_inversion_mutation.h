#ifndef SIMPLE_INVERSION_MUTATION_H
#define SIMPLE_INVERSION_MUTATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/mutation/mutation_op.h"

class SimpleInversionMutation final : public MutationOp {
public:
    explicit SimpleInversionMutation(Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> mutate(const HyperParams &params, const vector<Chromosome> &population) const override;

    unique_ptr<MutationOp> clone() const override;
};

#endif // SIMPLE_INVERSION_MUTATION_H
