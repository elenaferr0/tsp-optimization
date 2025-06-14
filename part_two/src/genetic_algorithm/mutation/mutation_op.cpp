#include "genetic_algorithm/mutation/mutation_op.h"
#include <stdexcept>

MutationOp::MutationOp(const Logger::Level log_level, const HyperParams &params)
    : log(log_level), params(params) {
}
