#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H
#include <utils/logger.h>
#include <vector>
#include <genetic_algorithm/hyper_params.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class MutationOp {
protected:
    const Logger log;

public:
    explicit MutationOp(const Logger& log);

    virtual ~MutationOp() = default;

    [[nodiscard]] virtual string name() const = 0;

    virtual vector<Chromosome> mutate(const HyperParams &params, const vector<Chromosome> &population) const = 0;
};

#endif // MUTATION_OPERATOR_H
