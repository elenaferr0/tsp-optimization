#include "genetic_algorithm/grid_search.h"

#include "genetic_algorithm/mutation/simple_inversion_mutation.h"
#include "genetic_algorithm/selection/n_tournament_selection.h"

struct Config {
    const HyperParams &hyper_params;
    const string names[4];
};

GridSearch::GridSearch(
    const string &instance_name,
    const vector<double> &mutation_rates,
    const vector<double> &parent_replacement_rates,
    const vector<int> &selection_tournament_sizes,
    const vector<pair<double, double> > &convex_hull_random_init_ratios
) : instance_name(instance_name),
    mutation_rates(mutation_rates),
    parent_replacement_rates(parent_replacement_rates),
    selection_tournament_sizes(selection_tournament_sizes),
    convex_hull_random_init_ratios(convex_hull_random_init_ratios),
    log(Logger::Level::INFO, "GridSearch") {
}

pair<Chromosome, HyperParams> GridSearch::run_experiment(
    const vector<shared_ptr<PopulationInitialization> > &initializations,
    const unique_ptr<SelectionOp> &selection,
    const unique_ptr<CrossoverOp> &crossover,
    const unique_ptr<MutationOp> &mutation,
    const unique_ptr<Replacement> &replacement,
    const vector<shared_ptr<StoppingCriterion> > &stopping,
    const double mutation_rate,
    const double parent_replacement_rate,
    const int selection_tournament_size,
    double convex_hull_init_ratio,
    double random_init_ratio
) const {
    const HyperParams params{
        .mutation_rate = mutation_rate,
        .parents_replacement_rate = parent_replacement_rate,
        .selection_tournament_size = selection_tournament_size,
        .convex_hull_random_init_ratio = {convex_hull_init_ratio, random_init_ratio}
    };

    params.validate_or_throw();

    GeneticAlgorithm ga(
        initializations,
        selection.get()->clone(),
        crossover.get()->clone(),
        mutation.get()->clone(),
        replacement.get()->clone(),
        stopping,
        log.get_min_level()
    );

    return {ga.start(params, instance_name, 10), params};
}

void GridSearch::run() const {
    auto graph = Graph(instance_name.c_str());
    auto log_level = log.get_min_level();

    vector<shared_ptr<PopulationInitialization> > initializations(2);
    initializations[0] = make_shared<ConvexHullInitialization>(log_level, graph);
    initializations[1] = make_shared<RandomInitialization>(log_level, graph);

    vector<unique_ptr<SelectionOp> > selection(2);
    selection[0] = make_unique<LinearRankingSelection>(log_level);
    selection[1] = make_unique<NTournamentSelection>(log_level);

    vector<unique_ptr<CrossoverOp> > crossover(2);
    crossover[0] = make_unique<OrderCrossover>(log_level);
    crossover[1] = make_unique<EdgeRecombinationCrossover>(log_level);

    vector<unique_ptr<MutationOp> > mutation(2);
    mutation[0] = make_unique<DisplacementMutation>(log_level);
    mutation[1] = make_unique<SimpleInversionMutation>(log_level);

    vector<unique_ptr<Replacement> > replacement(2);
    replacement[0] = make_unique<ElitismReplacement>(log_level);
    replacement[1] = make_unique<SteadyStateReplacement>(log_level);

    vector<shared_ptr<StoppingCriterion> > stopping_criteria;
    stopping_criteria.push_back(make_shared<MaxNonImprovingGenerationsCriterion>(log_level));
    stopping_criteria.push_back(make_shared<TimeLimitCriterion>(log_level));

    unique_ptr<pair<Chromosome, Config> > best = nullptr;

    for (const unique_ptr<SelectionOp> &sel: selection) {
        string sel_name = sel->name();
        for (const unique_ptr<CrossoverOp> &cross: crossover) {
            string cross_name = cross->name();
            for (const unique_ptr<MutationOp> &mut: mutation) {
                string mut_name = mut->name();
                for (const unique_ptr<Replacement> &repl: replacement) {
                    string repl_name = repl->name();
                    for (const auto &mutation_rate: mutation_rates) {
                        for (const auto &parent_replacement_rate: parent_replacement_rates) {
                            for (const auto &selection_tournament_size: selection_tournament_sizes) {
                                for (const auto &[ch_ratio, rnd_ratio]: convex_hull_random_init_ratios) {
                                    auto [chromosome, params] = run_experiment(
                                        initializations,
                                        sel,
                                        cross,
                                        mut,
                                        repl,
                                        stopping_criteria,
                                        mutation_rate,
                                        parent_replacement_rate,
                                        selection_tournament_size,
                                        ch_ratio,
                                        rnd_ratio
                                    );

                                    log.info("Fitness: " + to_string(chromosome.evaluate_fitness()) +
                                             " | " + "Selection: " + sel_name +
                                             ", Crossover: " + cross_name +
                                             ", Mutation: " + mut_name + " [rate: " + to_string(mutation_rate) + "]" +
                                             ", Replacement: " + repl_name + " [rate: " + to_string(
                                                 parent_replacement_rate) + "]" +
                                             ", Tournament size: " + to_string(selection_tournament_size) +
                                             ", Convex hull init ratio: " + to_string(ch_ratio) +
                                             ", Random init ratio: " + to_string(rnd_ratio));


                                    if (best == nullptr || chromosome.evaluate_fitness() < best->first.
                                        evaluate_fitness()) {
                                        auto config = Config{
                                            .hyper_params = params,
                                            .names = {
                                                sel_name,
                                                cross_name,
                                                mut_name,
                                                repl_name
                                            }
                                        };
                                        best = make_unique<pair<Chromosome, Config> >(chromosome, config);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (best == nullptr) {
        throw runtime_error("No valid solution found during grid search.");
    }

    log.info("Best found solution for instance " + instance_name + ":\n" +
             "Fitness: " + to_string(best->first.evaluate_fitness()) + "\n" +
             "Configuration:\n" +
             "  Selection: " + best->second.names[0] + "\n" +
             "  Crossover: " + best->second.names[1] + "\n" +
             "  Mutation: " + best->second.names[2] + " [rate: " + to_string(best->second.hyper_params.mutation_rate) +
             "]\n" +
             "  Replacement: " + best->second.names[3] + " [rate: " + to_string(
                 best->second.hyper_params.parents_replacement_rate) + "]\n" +
             "  Tournament size: " + to_string(best->second.hyper_params.selection_tournament_size) + "\n" +
             "  Convex hull init ratio: " + to_string(best->second.hyper_params.convex_hull_random_init_ratio.first) +
             "\n" +
             "  Random init ratio: " + to_string(best->second.hyper_params.convex_hull_random_init_ratio.second));

    best->first.save_to_file(instance_name);
}
