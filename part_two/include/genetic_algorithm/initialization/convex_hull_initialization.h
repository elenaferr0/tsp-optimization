#ifndef CONVEX_HULL_INITIALIZATION_H
#define CONVEX_HULL_INITIALIZATION_H

#include <random>

#include "population_initialization.h"

class ConvexHullInitialization final : public PopulationInitialization {
  Chromosome generate_chromosome() override;

  vector<Node> convex_hull() const;

  static bool in_hull(const Node &node, const vector<Node> &hull);
  pair<int, double> best_insertion(const vector<Node> &tour, const Node &node) const;
  double insertion_cost(const vector<Node> &tour, int pos, const Node &node) const;

  mutable mt19937 gen;
public:
  ConvexHullInitialization(Logger::Level log_level, Graph graph, int population_size);

  vector<Chromosome> generate_population() override;
};

#endif // CONVEX_HULL_INITIALIZATION_H
