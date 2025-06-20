#include "genetic_algorithm/stopping/time_limit_criterion.h"

TimeLimitCriterion::~TimeLimitCriterion() { delete start_time; }

unique_ptr<StoppingCriterion> TimeLimitCriterion::clone() const {
    return make_unique<TimeLimitCriterion>(*this);
}

TimeLimitCriterion::TimeLimitCriterion(const Logger::Level log_level)
    : StoppingCriterion(Logger(log_level, "TimeLimitCriterion")), start_time(nullptr) {
}

void TimeLimitCriterion::handle_start(const HyperParams& params) {
  start_time = new time_pt(chrono::high_resolution_clock::now());
}

bool TimeLimitCriterion::should_stop(const HyperParams& params, double best_fitness) {
  if (start_time == nullptr) {
    return false; // If the start time is not set, we cannot determine if we should stop
  }
  const auto current_time = chrono::high_resolution_clock::now();
  const auto elapsed_seconds =
      chrono::duration_cast<chrono::seconds>(current_time - *start_time)
          .count();
  return elapsed_seconds >= params.time_limit_seconds;
}