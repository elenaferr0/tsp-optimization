#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class CrossoverOp {
public:
    virtual ~CrossoverOp() = default;

    virtual void recombine(const vector<Chromosome> &parents) const = 0;
};

#endif //CROSSOVER_OPERATOR_H
