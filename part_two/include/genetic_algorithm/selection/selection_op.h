#ifndef SELECTION_OP_H
#define SELECTION_OP_H

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SelectionOp {
protected:
    int n_parents;

public:
    explicit SelectionOp(const int n_parents = 2) : n_parents(n_parents) {
    }

    virtual ~SelectionOp() = default;

    virtual vector<Chromosome> select(const vector<Chromosome> &population) = 0;
};

#endif //SELECTION_OP_H
