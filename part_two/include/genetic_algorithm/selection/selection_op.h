#ifndef SELECTION_OP_H
#define SELECTION_OP_H

#include <genetic_algorithm/hyper_params.h>
#include <utils/logger.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SelectionOp {
protected:
    const Logger log;

public:
    explicit SelectionOp(const Logger& log);

    virtual ~SelectionOp() = default;

    virtual string name() const = 0;

    virtual unique_ptr<SelectionOp> clone() const = 0;

    virtual vector<Chromosome> select(const HyperParams &params, const vector<Chromosome> &population) const = 0;
};

#endif // SELECTION_OP_H
