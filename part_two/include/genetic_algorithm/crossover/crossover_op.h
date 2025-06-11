#ifndef CROSSOVER_OPERATOR_H
#define CROSSOVER_OPERATOR_H

#include <utils/logger.h>
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class CrossoverOp {
protected:
  Logger log;

public:
  explicit CrossoverOp(Logger::Level log_level);
  virtual ~CrossoverOp() = default;

  virtual vector<Chromosome> recombine(const vector<Chromosome> &parents) = 0;
};

#endif // CROSSOVER_OPERATOR_H
