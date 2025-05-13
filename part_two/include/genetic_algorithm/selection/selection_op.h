#ifndef SELECTION_OP_H
#define SELECTION_OP_H

#include "utils/typedefs.h"

using namespace std;

class SelectionOp {
    protected:
    int n_parents;
public:
    explicit SelectionOp(const int n_parents = 2) : n_parents(n_parents) {}
    virtual ~SelectionOp() = default;
    virtual vector<ChromosomePtr> select(const vector<ChromosomePtr>& population) = 0;
};

#endif //SELECTION_OP_H
