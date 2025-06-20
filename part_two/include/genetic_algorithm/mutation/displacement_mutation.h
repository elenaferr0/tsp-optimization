#ifndef DISPLACEMENT_MUTATION_H
#define DISPLACEMENT_MUTATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/mutation/mutation_op.h"

class DisplacementMutation final : public MutationOp {
public:
    explicit DisplacementMutation(Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> mutate(const HyperParams &params, const vector<Chromosome> &population) const override;

    unique_ptr<MutationOp> clone() const override;
};

#endif // DISPLACEMENT_MUTATION_H
