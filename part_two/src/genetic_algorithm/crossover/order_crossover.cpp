#include "genetic_algorithm/crossover/order_crossover.h"
#include "tsp/graph.h"
#include "utils/path.h"
#include <algorithm>
#include <iostream>
#include <set>

OrderCrossover::OrderCrossover(const Logger::Level log_level)
    : CrossoverOp(log_level)
{
  log.set_label("OrderCrossover");
}

vector<Chromosome> OrderCrossover::recombine(const vector<Chromosome> &parents)
{
  const auto n_parents = parents.size();

  auto const n_genes = parents[0].get_n_genes();
  vector<Chromosome> result;

  for (int i = 0; i < n_parents; i += 2)
  {
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

    if (cut2 < cut1)
    {
      throw invalid_argument(
          "Cut points must be in increasing order, got cut1: " + to_string(cut1) +
          ", cut2: " + to_string(cut2));
    }

    vector<vector<Node>> offsprings(2, vector<Node>(n_genes));

    for (int j = 0; j < 2; ++j) // Both parents
    {
      vector<Node> subpath = current_pair[j].get_subpath(cut1, cut2);

      // Create a set of used nodes (from the subpath)
      set<int> used_nodes;
      for (const Node &node : subpath)
      {
        used_nodes.insert(node.id);
      }

      // First loop: Copy part before the cut1
      for (int pos = 0; pos < cut1; ++pos)
      {
        int idx = pos;
        int other_parent_idx = idx;

        // Find the next unused node from other parent
        while (used_nodes.find(current_pair[1 - j].get_node(other_parent_idx).id) != used_nodes.end())
        {
          other_parent_idx = (other_parent_idx + 1) % n_genes;
        }

        // Add this node to offspring
        offsprings[j][pos] = current_pair[1 - j].get_node(other_parent_idx);
        used_nodes.insert(offsprings[j][pos].id);
      }

      // Second loop: Copy the subpath from parent j
      for (int pos = cut1; pos <= cut2; ++pos)
      {
        offsprings[j][pos] = current_pair[j].get_node(pos);
      }

      // Third loop: Copy part after the cut2
      for (int pos = cut2 + 1; pos < n_genes; ++pos)
      {
        int idx = pos;
        int other_parent_idx = idx;

        // Find the next unused node from other parent
        while (used_nodes.find(current_pair[1 - j].get_node(other_parent_idx).id) != used_nodes.end())
        {
          other_parent_idx = (other_parent_idx + 1) % n_genes;
        }

        // Add this node to offspring
        offsprings[j][pos] = current_pair[1 - j].get_node(other_parent_idx);
        used_nodes.insert(offsprings[j][pos].id);
      }
    }

    // Add the offspring from this pair to results
    for (const auto &offspring : offsprings)
    {
      auto chromosome = Chromosome(Graph(offspring));
      if (chromosome.evaluate_fitness() < 0)
      {
        throw runtime_error("Offspring chromosome has negative fitness: " +
                            chromosome.to_str());
      }
      result.emplace_back(chromosome);
    }
  }

  log.trace("Selected " + to_string(n_parents) +
            " parents for crossover, generated " + to_string(result.size()) + " offspring");

  string fitnesses;
  for (const auto &offspring : result)
  {
    fitnesses += to_string(offspring.evaluate_fitness()) + " ";
  }
  log.trace("Fitnesses: " + fitnesses);

  return result;
}
