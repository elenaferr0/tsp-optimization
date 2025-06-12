#include "genetic_algorithm/crossover/order_crossover.h"
#include "tsp/graph.h"
#include "utils/path.h"
#include <algorithm>
#include <iostream>
#include <set>

OrderCrossover::OrderCrossover(const Logger::Level log_level)
    : CrossoverOp(log_level) {
  log.set_label("OrderCrossover");
}

vector<Chromosome> OrderCrossover::recombine(const vector<Chromosome> &parents) {
  const auto n_parents = parents.size();

  auto const n_genes = parents[0].get_n_genes();
  vector<Chromosome> result;

  for (int i = 0; i < n_parents; i += 2) {
    const int parent1_idx = i;
    const int parent2_idx = (i + 1 < n_parents) ? i + 1 : 0;

    vector<Chromosome> current_pair = {parents[parent1_idx], parents[parent2_idx]};

    // Apply existing crossover logic to the current pair
    vector<vector<Node>> offsprings(2, vector<Node>(n_genes));
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

    // Process both parents in the pair
    for (int j = 0; j < 2; ++j) {
      // Copy segments between cuts from parents to offsprings
      vector<Node> segment = current_pair[j].get_subpath(cut1, cut2);
      copy(segment.begin(), segment.end(), offsprings[j].begin() + cut1);

      set<int> used_nodes;
      transform(segment.begin(), segment.end(),
                inserter(used_nodes, used_nodes.end()),
                [](const Node &node) { return node.id; });

      // Fill the rest of the offspring with remaining nodes from the other
      // parent, in a circular fashion
      auto current_index = (cut2 + 1) % current_pair[j].get_n_genes();
      for (int k = 0; k < current_pair[j].get_n_genes(); ++k) {
        // Check whether the node has already been used
        if (used_nodes.find(current_pair[1 - j].get_node(current_index).id) ==
            used_nodes.end()) {
          offsprings[j][(cut2 + 1 + k) % current_pair[j].get_n_genes()] =
              current_pair[1 - j].get_node(current_index);
        }
        current_index = (current_index + 1) % current_pair[j].get_n_genes();
      }
    }

    // Add the offspring from this pair to results
    for (const auto &offspring : offsprings) {
      auto chromosome = Chromosome(Graph(offspring));
      result.emplace_back(chromosome);
    }
  }

  log.trace("Selected " + to_string(n_parents) +
            " parents for crossover, generated " + to_string(result.size()) + " offspring");

  string fitnesses;
  for (const auto &offspring : result) {
    fitnesses += to_string(offspring.evaluate_fitness()) + " ";
  }
  log.trace("Fitnesses: " + fitnesses);

  return result;
}
