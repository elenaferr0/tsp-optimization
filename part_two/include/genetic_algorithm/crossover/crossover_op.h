#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

class CrossoverOp {
public:
    virtual ~CrossoverOp() = default;

    virtual void recombine(const std::vector<Chromosome>& parents) = 0;
};

#endif //CROSSOVER_OPERATOR_H
