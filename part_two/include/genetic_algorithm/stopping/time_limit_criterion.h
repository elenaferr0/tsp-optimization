#ifndef TIME_LIMIT_CRITERIA_H
#define TIME_LIMIT_CRITERIA_H

#include <genetic_algorithm/hyper_params.h>

#include "stopping_criterion.h"
#include "utils/typedefs.h"

class TimeLimitCriterion final : public StoppingCriterion {
  time_pt *start_time;

public:
  TimeLimitCriterion(Logger::Level log_level, const HyperParams &params);
  bool should_stop(double best_fitness) override;
  void handle_start() override;
  ~TimeLimitCriterion() override;
};

#endif // TIME_LIMIT_CRITERIA_H
