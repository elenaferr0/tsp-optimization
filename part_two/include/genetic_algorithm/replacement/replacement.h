#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include <utils/logger.h>
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

class Replacement {
protected:
  Logger log;

public:
  explicit Replacement(Logger::Level log_level);

  virtual ~Replacement() = default;

  virtual vector<Chromosome>
  replace(const vector<Chromosome> &parents,
          const vector<Chromosome> &offsprings) const = 0;
};

#endif // REPLACEMENT_H
