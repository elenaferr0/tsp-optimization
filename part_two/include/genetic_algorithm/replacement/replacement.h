#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <utils/logger.h>
#include <vector>
#include <memory>
#include <genetic_algorithm/hyper_params.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class Replacement {
protected:
    const Logger log;

public:
    explicit Replacement(const Logger& log);

    virtual ~Replacement() = default;

    virtual string name() const = 0;

    virtual unique_ptr<Replacement> clone() const = 0;

    virtual vector<Chromosome> replace(const HyperParams &params, const vector<Chromosome> &parents,
                                       const vector<Chromosome> &offsprings) const = 0;
};

#endif // REPLACEMENT_H
