#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H

#include <utils/logger.h>
#include <vector>

#include "genetic_algorithm/hyper_params.h"
#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class CrossoverOp {
protected:
    const Logger log;

public:
    explicit CrossoverOp(const Logger& log);

    virtual ~CrossoverOp() = default;

    virtual string name() const = 0;

    virtual unique_ptr<CrossoverOp> clone() const = 0;

    virtual vector<Chromosome> recombine(const HyperParams &params, const vector<Chromosome> &parents) const = 0;
};

#endif // CROSSOVER_OPERATOR_H
