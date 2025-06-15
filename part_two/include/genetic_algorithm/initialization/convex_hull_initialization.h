#ifndef CONVEX_HULL_INITIALIZATION_H
#define CONVEX_HULL_INITIALIZATION_H

#include <random>
#include <vector>
#include "population_initialization.h"

class ConvexHullInitialization final : public PopulationInitialization {
    Chromosome generate_chromosome(const vector<Node> &convex_hull,
                                   const vector<Node> &interior_nodes);

    static vector<Node> compute_convex_hull(const vector<Node> &nodes);

    int find_best_insertion_position(const vector<Node> &current_tour, const Node& node_to_insert) const;

    double compute_insertion_cost(const vector<Node> &tour, const Node& node_to_insert, int position) const;

public:
    ConvexHullInitialization(Logger::Level log_level, const Graph& graph);

    vector<Chromosome> generate_population(const HyperParams& params) override;
};

#endif // CONVEX_HULL_INITIALIZATION_H
