#ifndef MAX_GENERATIONS_CRITERION_H
#define MAX_GENERATIONS_CRITERION_H

#include "stopping_criterion.h"

class MaxNonImprovingGenerationsCriterion final : public StoppingCriterion {
  long max_generations;
  double last_best_fitness;
  long non_improving_count;

public:
  MaxNonImprovingGenerationsCriterion(Logger::Level log_level, long max_generations);
  bool should_stop(double best_fitness = 0) override;
  void handle_start() override;
};

#endif // MAX_GENERATIONS_CRITERION_H
