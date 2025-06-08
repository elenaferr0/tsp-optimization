#ifndef SELECTION_OP_H
#define SELECTION_OP_H

#include <utils/logger.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SelectionOp {
protected:
    Logger log;
    int n_parents;

public:
    explicit SelectionOp( Logger::Level log_level, int n_parents = 2);
    virtual ~SelectionOp() = default;

    virtual vector<Chromosome> select(const vector<Chromosome> &population) = 0;
};

#endif //SELECTION_OP_H
