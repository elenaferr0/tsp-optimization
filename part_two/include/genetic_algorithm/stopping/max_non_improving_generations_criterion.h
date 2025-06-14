#ifndef MAX_GENERATIONS_CRITERION_H
#define MAX_GENERATIONS_CRITERION_H

#include "stopping_criterion.h"

class MaxNonImprovingGenerationsCriterion final : public StoppingCriterion {
  double last_best_fitness;
  long non_improving_count;

public:
  MaxNonImprovingGenerationsCriterion(Logger::Level log_level, const HyperParams &params);
  bool should_stop(double best_fitness = 0) override;
  void handle_start() override;
};

#endif // MAX_GENERATIONS_CRITERION_H
