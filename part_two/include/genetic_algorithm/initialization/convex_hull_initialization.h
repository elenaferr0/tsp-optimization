#ifndef CONVEX_HULL_INITIALIZATION_H
#define CONVEX_HULL_INITIALIZATION_H

#include <random>
#include <vector>
#include "population_initialization.h"

class ConvexHullInitialization final : public PopulationInitialization {
    Chromosome generate_chromosome(const vector<Node> &convex_hull,
                                   const vector<Node> &interior_points);

    static vector<Node> compute_convex_hull(const vector<Node> &nodes);

    int find_best_insertion_position(const vector<Node> &current_tour, int node_to_insert) const;

    double calculate_insertion_cost(const vector<Node> &tour, int node_to_insert, int position) const;

    mutable mt19937 gen;

public:
    ConvexHullInitialization(Logger::Level log_level, Graph graph, int population_size);

    vector<Chromosome> generate_population() override;
};

#endif // CONVEX_HULL_INITIALIZATION_H
