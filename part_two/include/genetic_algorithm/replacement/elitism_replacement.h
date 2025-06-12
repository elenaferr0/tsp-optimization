#ifndef ELITISM_REPLACEMENT_H
#define ELITISM_REPLACEMENT_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "replacement.h"

using namespace std;

class ElitismReplacement final : public Replacement {
  double ratio; // Ratio of parents to be replaced
public:
  explicit ElitismReplacement(Logger::Level log_level, double ratio = 0.05);

  vector<Chromosome> replace(const vector<Chromosome> &parents, const vector<Chromosome> &offsprings) override;
};

#endif // ELITISM_REPLACEMENT_H
