#include "genetic_algorithm/selection/selection_op.h"

SelectionOp::SelectionOp(Logger::Level log_level, int n_parents)
    : log(log_level), n_parents(n_parents) {}