#ifndef STOPPING_CRITERIA_H
#define STOPPING_CRITERIA_H
#include <genetic_algorithm/hyper_params.h>

#include "utils/logger.h"

class StoppingCriterion {
protected:
  Logger log;
  HyperParams params;

public:
  explicit StoppingCriterion(Logger::Level log_level, const HyperParams &params);
  virtual ~StoppingCriterion() = default;
  virtual void handle_start() = 0;
  // Best fitness will be used by MaxNonImprovingGenerationsCriterion
  virtual bool should_stop(double best_chromosome_fitness = 0) = 0;
};

#endif // STOPPING_CRITERIA_H
