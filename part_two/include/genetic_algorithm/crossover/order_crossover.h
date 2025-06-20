#ifndef ORDER_CROSSOVER_OPERATOR_H
#define ORDER_CROSSOVER_OPERATOR_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/crossover/crossover_op.h"
#include <vector>

using namespace std;

class OrderCrossover final : public CrossoverOp {
public:
    explicit OrderCrossover(Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> recombine(const HyperParams &params, const vector<Chromosome> &parents) const override;
};

#endif // ORDER_CROSSOVER_OPERATOR_H
