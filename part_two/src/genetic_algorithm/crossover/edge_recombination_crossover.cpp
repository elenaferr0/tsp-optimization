#include "genetic_algorithm/crossover/edge_recombination_crossover.h"
#include "tsp/graph.h"
#include "utils/path.h"
#include <set>
#include <vector>

EdgeRecombinationCrossover::EdgeRecombinationCrossover(const Logger::Level log_level)
    : CrossoverOp(Logger(log_level, name())) {
}

vector<Chromosome> EdgeRecombinationCrossover::recombine(const HyperParams &params,
                                                         const vector<Chromosome> &parents) const {
    const auto n_parents = parents.size();
    auto const n_genes = parents[0].get_n_genes();
    vector<Chromosome> result;

    vector<Chromosome> shuffled_parents = parents;
    shuffle(shuffled_parents.begin(), shuffled_parents.end(), std::mt19937{std::random_device{}()});

    for (int i = 0; i < n_parents; i += 2) {
        const int parent1_idx = i;
        const int parent2_idx = (i + 1 < n_parents) ? i + 1 : 0;
        const Chromosome &parent1 = shuffled_parents[parent1_idx];
        const Chromosome &parent2 = shuffled_parents[parent2_idx];

        // Build edge map
        map<int, set<int> > edge_map = build_edge_map({parent1, parent2});

        // Create ID to Node mapping
        map<int, Node> id_to_node;
        for (int j = 0; j < n_genes; j++) {
            Node node = parent1.get_node(j);
            id_to_node[node.id] = node;
        }

        int current_city_id = id_to_node[unif_int(0, n_genes - 1)].id;

        vector<Node> offspring;
        offspring.reserve(n_genes);
        set<int> visited_nodes;

        while (offspring.size() < n_genes) {
            // Add current city to offspring
            offspring.emplace_back(id_to_node[current_city_id]);
            visited_nodes.insert(current_city_id);

            // Remove current city from all edge lists
            for (auto &[_, edges]: edge_map) {
                edges.erase(current_city_id);
            }

            // Check if current city has any unvisited neighbors
            if (edge_map[current_city_id].empty()) {
                if (visited_nodes.size() == n_genes) {
                    break;
                }
                // Choose random unvisited city
                for (const auto &[id, _]: id_to_node) {
                    if (visited_nodes.find(id) == visited_nodes.end()) {
                        current_city_id = id;
                        break;
                    }
                }
                continue;
            }

            // Find next city with minimum edges
            int next_city_id = -1;
            int min_edges = numeric_limits<int>::max();

            for (auto id: edge_map[current_city_id]) {
                int edges_count = static_cast<int>(edge_map[id].size());
                if (edges_count < min_edges) {
                    min_edges = edges_count;
                    next_city_id = id;
                } else if (edges_count == min_edges && unif_real(0.0, 1.0) < 0.5) {
                    next_city_id = id;
                }
            }

            current_city_id = next_city_id;
        }

        Graph graph(offspring);
        result.emplace_back(graph);
    }
    return result;
}

map<int, set<int> > EdgeRecombinationCrossover::build_edge_map(const vector<Chromosome> &parents) {
    map<int, set<int> > edge_map;

    for (const auto &parent: parents) {
        for (int i = 0; i < parent.get_n_genes(); ++i) {
            int current_node = parent.get_node(i).id;
            int next_node = parent.get_node((i + 1) % parent.get_n_genes()).id;

            // Add the edge in both directions
            edge_map[current_node].insert(next_node);
            edge_map[next_node].insert(current_node);
        }
    }

    return edge_map;
}

string EdgeRecombinationCrossover::name() const {
    return "EdgeRecombinationCrossover";
}

unique_ptr<CrossoverOp> EdgeRecombinationCrossover::clone() const {
    return make_unique<EdgeRecombinationCrossover>(*this);
}
