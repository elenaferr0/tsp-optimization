#ifndef SELECTION_OP_H
#define SELECTION_OP_H

#include <genetic_algorithm/hyper_params.h>
#include <utils/logger.h>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class SelectionOp {
protected:
  Logger log;

public:
  explicit SelectionOp(Logger::Level log_level);
  virtual ~SelectionOp() = default;

  virtual vector<Chromosome> select(const HyperParams& params,const vector<Chromosome> &population) = 0;
};

#endif // SELECTION_OP_H
