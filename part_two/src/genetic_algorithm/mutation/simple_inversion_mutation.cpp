#include "genetic_algorithm/mutation/simple_inversion_mutation.h"
#include "utils/maths.h"
#include "utils/path.h"

SimpleInversionMutation::SimpleInversionMutation(const Logger::Level log_level)
    : MutationOp(Logger(log_level, name())) {
}

vector<Chromosome> SimpleInversionMutation::mutate(const HyperParams &params, const vector<Chromosome> &population) const {
    vector<Chromosome> mutated_population;
    mutated_population.reserve(population.size());

    for (const auto &chromosome: population) {
        if (unif_real(0.0, 1.0) > params.mutation_rate) {
            mutated_population.push_back(chromosome);
            continue; // Skip mutation
        }

        auto cuts = random_cut_points(chromosome.get_n_genes());
        const auto subpath_start = cuts.top();
        cuts.pop();
        const auto subpath_end = cuts.top();
        cuts.pop();
        log.trace("Simple inversion mutation: start: " + to_string(subpath_start) + ", end: " + to_string(subpath_end));

        auto subpath = chromosome.get_subpath(subpath_start, subpath_end);
        vector<Node> mutated_genes(chromosome.get_n_genes());

        // Copy the genes before the subpath
        for (int i = 0; i < subpath_start; ++i) {
            mutated_genes[i] = chromosome.get_node(i);
        }
        // Copy the subpath in reverse order
        for (int i = 0; i < subpath.size(); ++i) {
            mutated_genes[subpath_start + i] = subpath[subpath.size() - 1 - i];
        }
        // Copy the genes after the subpath
        for (int i = subpath_end + 1; i < chromosome.get_n_genes(); ++i) {
            mutated_genes[i - (subpath_end - subpath_start + 1)] = chromosome.get_node(i);
        }

        mutated_population.emplace_back(Graph(mutated_genes));
    }

    if (log.get_min_level() <= Logger::Level::TRACE) {
        string fitnesses;
        for (const auto &chromosome: mutated_population) {
            fitnesses += chromosome.to_str() + " ";
        }
        log.trace("Fitnesses: " + fitnesses);
    }

    return mutated_population;
}

unique_ptr<MutationOp> SimpleInversionMutation::clone() const {
    return make_unique<SimpleInversionMutation>(*this);
}

string SimpleInversionMutation::name() const {
    return "SimpleInversionMutation";
}
