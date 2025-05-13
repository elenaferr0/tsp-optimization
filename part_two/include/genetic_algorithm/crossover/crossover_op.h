#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

class CrossoverOp {
public:
    void recombine(const std::vector<Chromosome>& parents);
};

#endif //CROSSOVER_OPERATOR_H
