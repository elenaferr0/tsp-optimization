#ifndef ORDER_CROSSOVER_OPERATOR_H
#define ORDER_CROSSOVER_OPERATOR_H

#include <vector>
#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/crossover/crossover_op.h"

using namespace std;

class OrderCrossover final : public CrossoverOp {
public:
    OrderCrossover() = default;

    vector<Chromosome> recombine(const vector<Chromosome> &parents) const override;
};

#endif //ORDER_CROSSOVER_OPERATOR_H
