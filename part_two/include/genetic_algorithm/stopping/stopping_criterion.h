#ifndef STOPPING_CRITERIA_H
#define STOPPING_CRITERIA_H
#include <genetic_algorithm/hyper_params.h>
#include <memory>

#include "utils/logger.h"

using namespace std;

class StoppingCriterion {
protected:
    const Logger log;

public:
    explicit StoppingCriterion(const Logger &log);

    virtual ~StoppingCriterion() = default;

    virtual void handle_start(const HyperParams &params) = 0;

    virtual unique_ptr<StoppingCriterion> clone() const = 0;

    // Best fitness will be used by MaxNonImprovingGenerationsCriterion
    virtual bool should_stop(const HyperParams &params, double best_chromosome_fitness = 0) = 0;
};

#endif // STOPPING_CRITERIA_H
