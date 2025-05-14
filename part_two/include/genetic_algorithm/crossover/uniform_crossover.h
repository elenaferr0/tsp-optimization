#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H
#include <vector>
#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/crossover/crossover_op.h"

using namespace std;

class UniformCrossover final : public CrossoverOp {
public:
    UniformCrossover() = default;

   void recombine(const vector<Chromosome> &parents) const override;
};

#endif //CROSSOVER_OPERATOR_H
