#include "genetic_algorithm/selection/selection_op.h"

SelectionOp::SelectionOp(const Logger::Level log_level, const int n_parents)
    : log(log_level), n_parents(n_parents) {}