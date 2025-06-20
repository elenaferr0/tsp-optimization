#include "genetic_algorithm/initialization/convex_hull_initialization.h"

#include <algorithm>
#include <set>
#include <limits>

#include "genetic_algorithm/chromosome/chromosome.h"
#include "utils/maths.h"

ConvexHullInitialization::ConvexHullInitialization(const Logger::Level log_level, const Graph &graph)
    : PopulationInitialization(Logger(log_level, "ConvexHullInitialization"), graph) {
}

vector<Chromosome> ConvexHullInitialization::generate_population(const HyperParams &params) const {
    vector<Chromosome> population;

    // Get the convex hull once
    auto convex_hull = compute_convex_hull(graph.path);

    // Get interior points (nodes not on convex hull)
    set<int> hull_ids;
    for (const auto &node: convex_hull) {
        hull_ids.insert(node.id);
    }

    vector<Node> interior_points;
    for (const auto &node: graph.path) {
        if (hull_ids.find(node.id) == hull_ids.end()) {
            interior_points.push_back(node);
        }
    }

    const int to_generate = floor(params.population_size * params.convex_hull_random_init_ratio.first);
    for (int i = 0; i < to_generate; ++i) {
        population.push_back(generate_chromosome(convex_hull, interior_points));
    }

    return population;
}

Chromosome ConvexHullInitialization::generate_chromosome(const vector<Node> &convex_hull,
                                                         const vector<Node> &interior_nodes) const {
    vector<Node> tour;
    for (const auto &node: convex_hull) {
        tour.push_back(node);
    }

    vector<Node> interior_nodes_copy = interior_nodes;
    shuffle(interior_nodes_copy.begin(), interior_nodes_copy.end(), gen);
    for (const auto &node: interior_nodes_copy) {
        const auto best_index = find_best_insertion_position(tour, node);
        tour.insert(tour.begin() + best_index + 1, node);
    }

    const Graph graph_tour(tour);
    return Chromosome(graph_tour);
}

int ConvexHullInitialization::find_best_insertion_position(const vector<Node> &current_tour,
                                                           const Node &node_to_insert) const {
    double min_cost_increase = numeric_limits<double>::max();
    int best_position = 0;

    // Try inserting between each pair of consecutive cities
    for (int i = 0; i <= current_tour.size(); ++i) {
        const double cost_increase = compute_insertion_cost(current_tour, node_to_insert, i);

        if (cost_increase < min_cost_increase) {
            min_cost_increase = cost_increase;
            best_position = i;
        }
    }

    return best_position;
}

double ConvexHullInitialization::compute_insertion_cost(const vector<Node> &tour, const Node &node_to_insert,
                                                        const int position) const {
    const int tour_size = tour.size();

    if (tour_size == 0)
        return 0.0;
    if (tour_size == 1) {
        // Cost of going from tour[0] to node_to_insert and back
        return 2.0 * graph.get_cost(tour[0].id, node_to_insert.id);
    }

    // Get the nodes before and after insertion position
    const Node prev_node = (position == 0) ? tour[tour_size - 1] : tour[position - 1];
    const Node next_node = (position == tour_size) ? tour[0] : tour[position];

    // Calculate cost increase: new_edges - old_edge
    const double old_cost = graph.get_cost(prev_node.id, next_node.id);
    const double new_cost = graph.get_cost(prev_node.id, node_to_insert.id) +
                            graph.get_cost(node_to_insert.id, next_node.id);

    return new_cost - old_cost;
}

vector<Node> ConvexHullInitialization::compute_convex_hull(const vector<Node> &nodes) {
    if (nodes.size() < 3)
        return nodes;

    // Graham scan algorithm
    vector<Node> points = nodes;

    // Find the bottom-most point (or left most in case of tie)
    int min_idx = 0;
    for (int i = 1; i < points.size(); ++i) {
        if (points[i].y() < points[min_idx].y() ||
            (points[i].y() == points[min_idx].y() &&
             points[i].x() < points[min_idx].x())) {
            min_idx = i;
        }
    }
    // swap the bottom-most point with the first point
    const Node tmp = points[0];
    points[0] = points[min_idx];
    points[min_idx] = tmp;

    // Sort points by polar angle with respect to points[0]
    Node pivot = points[0];
    sort(points.begin() + 1, points.end(),
         [&pivot](const Node &a, const Node &b) {
             const double angle_a = atan2(a.y() - pivot.y(), a.x() - pivot.x());
             const double angle_b = atan2(b.y() - pivot.y(), b.x() - pivot.x());
             return angle_a < angle_b;
         });

    // Build convex hull
    vector<Node> hull;
    for (const auto &point: points) {
        // Remove points that make clockwise turn
        while (hull.size() >= 2 &&
               cross_product(hull[hull.size() - 2], hull[hull.size() - 1], point) <= 0) {
            hull.pop_back();
        }
        hull.push_back(point);
    }

    return hull;
}
