#ifndef MAX_GENERATIONS_CRITERION_H
#define MAX_GENERATIONS_CRITERION_H

#include "stopping_criterion.h"

class MaxGenerationsCriterion final : public StoppingCriterion {
  long max_generations;
  long current_generation;

public:
  MaxGenerationsCriterion(Logger::Level log_level, long time_limit_seconds);
  bool should_stop() override;
  void handle_start() override;
};

#endif // MAX_GENERATIONS_CRITERION_H
