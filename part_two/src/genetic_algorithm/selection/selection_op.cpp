#include "genetic_algorithm/selection/selection_op.h"

#include <genetic_algorithm/hyper_params.h>

SelectionOp::SelectionOp(const Logger::Level log_level, const HyperParams &params)
    : log(log_level), params(params) {
}
