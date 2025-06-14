#include "genetic_algorithm/stopping/time_limit_criterion.h"

TimeLimitCriterion::~TimeLimitCriterion() { delete start_time; }

TimeLimitCriterion::TimeLimitCriterion(const Logger::Level log_level, const HyperParams &params)
    : StoppingCriterion(log_level, params), start_time(nullptr) {
  log.set_label("TimeLimitCriterion");
}

void TimeLimitCriterion::handle_start() {
  start_time = new time_pt(chrono::high_resolution_clock::now());
}

bool TimeLimitCriterion::should_stop(double best_fitness) {
  if (start_time == nullptr) {
    return false; // If the start time is not set, we cannot determine if we should stop
  }
  const auto current_time = chrono::high_resolution_clock::now();
  const auto elapsed_seconds =
      chrono::duration_cast<chrono::seconds>(current_time - *start_time)
          .count();
  return elapsed_seconds >= params.time_limit_seconds;
}