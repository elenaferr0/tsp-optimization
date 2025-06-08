#include "genetic_algorithm/crossover/order_crossover.h"
#include "tsp/graph.h"
#include "utils/maths.h"
#include "utils/path.h"
#include <algorithm>
#include <set>

vector<Chromosome> OrderCrossover::recombine(const vector<Chromosome> &parents) const {
  int n_parents = parents.size();
  if (n_parents != 2) {
    throw invalid_argument("OrderCrossover requires two parents");
  }

  // Temporarily use vector<Node> instead of Chromosome for simpler handling
  vector<vector<Node>> offsprings(n_parents, vector<Node>(parents[0].get_n_genes()));
  auto cuts = random_cut_points(parents.size());
  cout << "Cut points: ";
  priority_queue_asc<int> cuts_copy = cuts; // Copy for output
  while (!cuts_copy.empty()) {
    cout << cuts_copy.top() << " ";
    cuts_copy.pop();
  }
  cout << endl;

  for (int i = 0; i < n_parents; ++i) {
    int cut1 = cuts.top();
    cuts.pop();
    int cut2 = cuts.top();
    cuts.pop();

    if (cut2 > cut1) {
       throw invalid_argument("Cut points must be in increasing order");
    }

    // Copy segments between cuts from parents to offsprings
    vector<Node> segment = parents[i].get_subpath(cut1, cut2);
    copy(segment.begin(), segment.end(), offsprings[i].begin() + cut1);

    set<int> used_nodes;
    transform(segment.begin(), segment.end(), inserter(used_nodes, used_nodes.end()),
           [](const Node &node) { return node.id; });

    // Fill the rest of the offspring with remaining nodes from the other parent, in a circular fashion
    int current_index = (cut2 + 1) % parents[i].get_n_genes();
    for (int j = 0; j < parents[i].get_n_genes(); ++j) {
      // Check whether the node has already been used
      if (used_nodes.find(parents[1 - i].get_node(current_index).id) == used_nodes.end()) {
        offsprings[i][(cut2 + 1 + j) % parents[i].get_n_genes()] = parents[1 - i].get_node(current_index);
      }
      current_index = (current_index + 1) % parents[i].get_n_genes();
    }
  }

  vector<Chromosome> result;
  for (const auto &offspring : offsprings) {
    result.push_back(Chromosome(Graph(offspring)));
  }
  return result;
}
