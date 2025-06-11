#include "genetic_algorithm/initialization/convex_hull_initialization.h"
#include "utils/maths.h"
#include <algorithm>
#include <stack>

ConvexHullInitialization::ConvexHullInitialization(Logger::Level log_level, Graph graph, int population_size)
    : PopulationInitialization(log_level, graph, population_size) {
    random_device rd;
    gen = mt19937(rd());
}

vector<Chromosome> ConvexHullInitialization::generate_population() {
    if (graph.path.empty()) {
        throw runtime_error("Graph path is empty, cannot generate population.");
    }

    vector<Chromosome> population;
    for (int i = 0; i < population_size; ++i) {
        population.push_back(generate_chromosome());
    }

    return population;
}

Chromosome ConvexHullInitialization::generate_chromosome() {
    const vector<Node> hull = convex_hull();
    vector<Node> tour(hull);

    vector<Node> interior_nodes;
    for (const Node &node: graph.path) {
        if (!in_hull(node, hull)) {
            interior_nodes.push_back(node);
        }
    }

    shuffle(interior_nodes.begin(), interior_nodes.end(), gen);

    for (const Node &node: interior_nodes) {
        auto [best_index, cost] = best_insertion(tour, node);
        tour.insert(tour.begin() + best_index + 1, node);
    }

    Graph tour_graph(tour);
    return Chromosome(tour_graph);
}

vector<Node> ConvexHullInitialization::convex_hull() const {
    if (graph.path.empty()) {
        throw runtime_error("Graph path is empty, cannot perform Graham scan.");
    }

    if (graph.path.size() < 3) {
        return graph.path;
    }

    vector<Node> nodes(graph.path);
    int n = nodes.size();

    // Find the bottom-most point (and leftmost in case of tie)
    int min_idx = 0;
    for (int i = 1; i < nodes.size(); i++) {
        if (nodes[i].position < nodes[min_idx].position) {
            min_idx = i;
        }
    }

    Node pivot = nodes[min_idx];
    swap(nodes[0], nodes[min_idx]); // Move pivot to the front

    // Sort points by polar angle with respect to pivot
    sort(nodes.begin() + 1, nodes.end(), [&pivot](const Node &a, const Node &b) {
        return polar_compare(pivot, a, b);
    });

    // if several points have the same polar angle then only keep the farthest
    vector<Node> filtered;
    filtered.push_back(pivot);
    for (int i = 1; i < n; i++) {
        // Skip points that are too close or have same angle (keep farthest for last
        // group)
        while (i < n - 1 &&
               abs(polar_angle(pivot.position, nodes[i].position) -
                   polar_angle(pivot.position, nodes[i + 1].position)) < 1e-9) {
            i++;
        }
        filtered.push_back(nodes[i]);
    }

    if (filtered.size() < 3) {
        return filtered; // Not enough points to form a hull
    }

    stack<Node> hull;
    hull.push(filtered[0]); // Pivot
    hull.push(filtered[1]); // First point
    for (int i = 2; i < filtered.size(); i++) {
        // Pop points that create clockwise turn
        while (hull.size() > 1) {
            Node top = hull.top();
            hull.pop();
            Node secondTop = hull.top();

            if (cross_product(secondTop, top, filtered[i]) > 0) {
                hull.push(top); // Put it back, it's a valid point
                break;
            }
            // Otherwise, keep the point popped and continue
        }
        hull.push(filtered[i]);
    }

    // Convert stack to vector
    vector<Node> result;
    while (!hull.empty()) {
        result.push_back(hull.top());
        hull.pop();
    }

    // Reverse to get counter-clockwise order
    std::reverse(result.begin(), result.end());
    return result;
}

bool ConvexHullInitialization::in_hull(const Node &node,
                                       const vector<Node> &hull) {
    if (hull.size() < 3) {
        return false; // Not enough points to form a hull
    }

    return any_of(hull.begin(), hull.end(),
                  [&node](const Node &h) { return h.id == node.id; });
}

pair<int, double> ConvexHullInitialization::best_insertion(const vector<Node> &tour, const Node &node) const {
    int best_index = -1;
    double min_cost = numeric_limits<double>::max();

    for (size_t i = 0; i < tour.size(); i++) {
        const double cost = insertion_cost(tour, i, node);
        if (cost < min_cost) {
            min_cost = cost;
            best_index = i;
        }
    }

    return {best_index, min_cost};
}

double ConvexHullInitialization::insertion_cost(const vector<Node> &tour, int pos, const Node &node) const {
    int n = tour.size();
    const Node &prev = tour[pos];
    const Node &next = tour[(pos + 1) % n];

    // dist(prev, node) + dist(node, next) - dist(prev, next)
    return graph.get_cost(prev.id, node.id) +
           graph.get_cost(node.id, next.id) -
           graph.get_cost(prev.id, next.id);
}
