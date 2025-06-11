#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "utils/maths.h"
#include "utils/path.h"

DisplacementMutation::DisplacementMutation(const Logger::Level log_level,
                                           const double rate)
    : MutationOp(log_level, rate) {
  log.set_label("DisplacementMutation");
}

vector<Chromosome>
DisplacementMutation::mutate(const vector<Chromosome> &population) const {
  vector<Chromosome> mutated_population;
  mutated_population.reserve(population.size());

  for (const auto &chromosome : population) {
    if (unif(0.0, 1.0) > rate) {
      mutated_population.push_back(chromosome);
      continue; // Skip mutation
    }

    auto cuts = random_cut_points(chromosome.get_n_genes());
    const auto subpath_start = cuts.top();
    cuts.pop();
    const auto subpath_end = cuts.top();
    cuts.pop();

    const int new_length =
        chromosome.get_n_genes() - (subpath_end - subpath_start + 1);
    if (new_length - 2 <= 0) {
      // If the subpath is the entire chromosome, skip mutation
      mutated_population.push_back(chromosome);
      continue;
    }

    int new_subpath_start = subpath_start; // Where to move the subpath
    while (new_subpath_start == subpath_start) {
      new_subpath_start = unif(0, new_length - 1);
    }

    // Move the subpath to the new position and copy others in order
    auto subpath = chromosome.get_subpath(subpath_start, subpath_end);
    vector<Node> mutated_genes(chromosome.get_n_genes());
    // Copy the subpath to the new position
    for (int i = 0; i < subpath.size(); ++i) {
      mutated_genes[(new_subpath_start + i) % chromosome.get_n_genes()] =
          subpath[i];
    }
    // Copy the rest of the genes in order, skipping the moved subpath
    int current_index = 0;
    for (int i = 0; i < chromosome.get_n_genes(); ++i) {
      if (i >= new_subpath_start && i < new_subpath_start + subpath.size()) {
        continue; // Skip the moved subpath
      }
      while (current_index < chromosome.get_n_genes() &&
             (current_index >= subpath_start && current_index <= subpath_end)) {
        current_index++;
      }
      mutated_genes[i] = chromosome.get_node(current_index);
      current_index++;
    }

    mutated_population.emplace_back(Graph(mutated_genes));
  }

  return mutated_population;
}
