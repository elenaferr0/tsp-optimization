#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H
#include <vector>
#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class MutationOp {
    double rate;
public:
    explicit MutationOp(double rate);
    virtual ~MutationOp() = default;

    virtual vector<Chromosome> mutate(const vector<Chromosome> &population) const = 0;
};

#endif //MUTATION_OPERATOR_H
