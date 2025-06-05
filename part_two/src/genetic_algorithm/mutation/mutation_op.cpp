#include "genetic_algorithm/mutation/mutation_op.h"
#include <stdexcept>

MutationOp::MutationOp(double rate) : rate(rate) {
  if (rate < 0.0 || rate > 1.0) {
    throw invalid_argument("Mutation rate must be between 0.0 and 1.0");
  }
}
