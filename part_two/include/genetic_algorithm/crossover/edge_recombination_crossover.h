#ifndef EDGE_RECOMBINATION_CROSSOVER_H
#define EDGE_RECOMBINATION_CROSSOVER_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/crossover/crossover_op.h"
#include <vector>
#include <map>
#include <set>

using namespace std;

class EdgeRecombinationCrossover final : public CrossoverOp {
  static map<int, set<int>> build_edge_map(const vector<Chromosome> &parents);
public:
  explicit EdgeRecombinationCrossover(Logger::Level log_level);

  vector<Chromosome> recombine(const vector<Chromosome> &parents) override;
};

#endif // EDGE_RECOMBINATION_CROSSOVER_H
