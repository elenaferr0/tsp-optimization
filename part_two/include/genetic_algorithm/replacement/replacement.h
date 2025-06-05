#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <vector>

#include "utils/typedefs.h"

using namespace std;

class Replacement {
public:
    Replacement() = default;

    virtual ~Replacement() = default;

    virtual vector<Chromosome> replace(const vector<Chromosome> &parents,
                                          const vector<Chromosome> &offsprings) const = 0;
};

#endif //REPLACEMENT_H
