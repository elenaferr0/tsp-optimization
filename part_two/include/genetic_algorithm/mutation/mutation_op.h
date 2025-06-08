#ifndef MUTATION_OPERATOR_H
#define MUTATION_OPERATOR_H
#include <vector>
#include <utils/logger.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class MutationOp {
protected:
    double rate;
    Logger log;
public:
    explicit MutationOp(Logger::Level log_level, double rate = 0.01);
    virtual ~MutationOp() = default;

    virtual vector<Chromosome> mutate(const vector<Chromosome> &population) const = 0;
};

#endif //MUTATION_OPERATOR_H
