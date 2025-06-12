#ifndef STOPPING_CRITERIA_H
#define STOPPING_CRITERIA_H
#include "utils/logger.h"

class StoppingCriterion {
protected:
  mutable Logger log;

public:
  explicit StoppingCriterion(Logger::Level log_level);
  virtual ~StoppingCriterion() = default;
  virtual void handle_start() = 0;
  // Best fitness will be used by MaxNonImprovingGenerationsCriterion
  virtual bool should_stop(double best_fitness = 0) = 0;
};

#endif // STOPPING_CRITERIA_H
