#ifndef MAX_GENERATIONS_CRITERION_H
#define MAX_GENERATIONS_CRITERION_H

#include "stopping_criterion.h"

class MaxNonImprovingGenerationsCriterion final : public StoppingCriterion {
    double last_best_fitness;
    long non_improving_count;

public:
    explicit MaxNonImprovingGenerationsCriterion(Logger::Level log_level);

    bool should_stop(const HyperParams &params, double best_fitness = 0) override;

    void handle_start(const HyperParams &params) override;

    unique_ptr<StoppingCriterion> clone() const override;
};

#endif // MAX_GENERATIONS_CRITERION_H
