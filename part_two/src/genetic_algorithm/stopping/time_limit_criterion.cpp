#include "genetic_algorithm/stopping/time_limit_criterion.h"

TimeLimitCriterion::~TimeLimitCriterion() {
    delete start_time;
}

TimeLimitCriterion::TimeLimitCriterion(const Logger::Level log_level, long time_limit_seconds)
    : StoppingCriterion(log_level), time_limit_seconds(time_limit_seconds), start_time(nullptr) {
    if (time_limit_seconds <= 0) {
        throw std::invalid_argument("time_limit_seconds must be greater than 0");
    }
    log.set_label("TimeLimitCriterion");
}

void TimeLimitCriterion::handle_start() {
  start_time = new time_pt(std::chrono::high_resolution_clock::now());
}

bool TimeLimitCriterion::should_stop() const {
    if (start_time == nullptr) {
        return false; // If the start time is not set, we cannot determine if we should stop
    }
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_seconds = std::chrono::duration_cast<std::chrono::seconds>(current_time - *start_time).count();
    return elapsed_seconds >= time_limit_seconds;
}