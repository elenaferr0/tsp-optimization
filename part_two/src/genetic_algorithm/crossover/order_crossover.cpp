#include "genetic_algorithm/crossover/order_crossover.h"
#include "tsp/graph.h"
#include "utils/path.h"
#include <set>
#include <vector>

OrderCrossover::OrderCrossover(const Logger::Level log_level)
    : CrossoverOp(log_level) {
    log.set_label("OrderCrossover");
}

vector<Chromosome> OrderCrossover::recombine(const HyperParams& params, const vector<Chromosome> &parents) {
    const auto n_parents = parents.size();
    auto const n_genes = parents[0].get_n_genes();
    vector<Chromosome> result;
    result.reserve(n_parents);

    for (int i = 0; i < n_parents; i += 2) {
        const int parent1_idx = i;
        const int parent2_idx = (i + 1 < n_parents) ? i + 1 : 0;
        vector<Chromosome> current_pair = {parents[parent1_idx], parents[parent2_idx]};

        auto cuts = random_cut_points(n_genes);
        const int cut1 = cuts.top();
        cuts.pop();
        const int cut2 = cuts.top();
        cuts.pop();

        log.trace("Cut points selected for pair (" + to_string(parent1_idx) +
                  ", " + to_string(parent2_idx) + "): cut1 = " + to_string(cut1) +
                  ", cut2 = " + to_string(cut2));

        if (cut2 < cut1) {
            throw invalid_argument(
                "Cut points must be in increasing order, got cut1: " + to_string(cut1) +
                ", cut2: " + to_string(cut2));
        }

        vector<vector<Node> > offsprings(2, vector<Node>(n_genes));

        for (int j = 0; j < 2; ++j) // Both parents
        {
            vector<Node> subpath = current_pair[j].get_subpath(cut1, cut2);

            set<int> used_nodes;
            for (const Node &node: subpath) {
                used_nodes.insert(node.id);
            }

            // Copy the subpath from current parent to offspring
            for (int pos = cut1; pos <= cut2; ++pos) {
                offsprings[j][pos] = current_pair[j].get_node(pos);
            }

            // Create a list of unused nodes from the other parent in their original order
            vector<Node> unused_nodes;
            for (int pos = 0; pos < n_genes; ++pos) {
                Node node = current_pair[1 - j].get_node(pos);
                if (used_nodes.find(node.id) == used_nodes.end()) {
                    unused_nodes.push_back(node);
                }
            }

            // Fill the remaining positions in order
            int unused_idx = 0;

            // Fill positions before cut1
            for (int pos = 0; pos < cut1; ++pos) {
                if (unused_idx < unused_nodes.size()) {
                    offsprings[j][pos] = unused_nodes[unused_idx++];
                }
            }

            // Fill positions after cut2
            for (int pos = cut2 + 1; pos < n_genes; ++pos) {
                if (unused_idx < unused_nodes.size()) {
                    offsprings[j][pos] = unused_nodes[unused_idx++];
                }
            }
        }

        // Add the offspring from this pair to results
        for (const auto &offspring: offsprings) {
            auto chromosome = Chromosome(Graph(offspring));
            if (chromosome.evaluate_fitness() < 0) {
                throw runtime_error("Offspring chromosome has negative fitness: " +
                                    chromosome.to_str());
            }
            result.emplace_back(chromosome);
        }
    }

    log.trace("Selected " + to_string(n_parents) +
              " parents for crossover, generated " + to_string(result.size()) + " offspring");

    if (log.get_min_level() <= Logger::Level::TRACE) {
        string fitnesses;
        for (const auto &offspring: result) {
            fitnesses += to_string(offspring.evaluate_fitness()) + " ";
        }
        log.trace("Fitnesses: " + fitnesses);
    }

    return result;
}
