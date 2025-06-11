#ifndef TIME_LIMIT_CRITERIA_H
#define TIME_LIMIT_CRITERIA_H

#include "stopping_criterion.h"
#include "utils/typedefs.h"

class TimeLimitCriterion final : public StoppingCriterion {
  long time_limit_seconds;
  time_pt *start_time;

public:
  TimeLimitCriterion(Logger::Level log_level, long time_limit_seconds);
  bool should_stop() override;
  void handle_start() override;
  ~TimeLimitCriterion() override;
};

#endif // TIME_LIMIT_CRITERIA_H
