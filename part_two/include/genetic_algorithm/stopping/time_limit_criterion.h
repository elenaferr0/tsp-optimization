#ifndef TIME_LIMIT_CRITERIA_H
#define TIME_LIMIT_CRITERIA_H

#include <genetic_algorithm/hyper_params.h>

#include "stopping_criterion.h"
#include "utils/typedefs.h"

class TimeLimitCriterion final : public StoppingCriterion {
    time_pt *start_time;

public:
    explicit TimeLimitCriterion(Logger::Level log_level);

    bool should_stop(const HyperParams &params, double best_fitness) override;

    void handle_start(const HyperParams &params) override;

    ~TimeLimitCriterion() override;
};

#endif // TIME_LIMIT_CRITERIA_H
