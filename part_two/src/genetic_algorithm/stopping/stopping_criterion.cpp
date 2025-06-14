#include "genetic_algorithm/stopping/stopping_criterion.h"

StoppingCriterion::StoppingCriterion(const Logger::Level log_level, const HyperParams &params)
    : log(log_level), params(params) {}