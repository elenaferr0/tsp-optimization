#include <iostream>
#include <memory>

#include "tsp/graph.h"
#include <genetic_algorithm/crossover/order_crossover.h>
#include <utils/path.h>

#include "genetic_algorithm/genetic_algorithm.h"
#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "genetic_algorithm/replacement/steady_state_replacement.h"
#include "genetic_algorithm/selection/linear_ranking_selection.h"
#include "genetic_algorithm/stopping/time_limit_criterion.h"

class SelectionOp;
using namespace std;

Chromosome create_chromosome(const int size, const int min_id = 0) {
    // Create a simple graph with 'size' nodes
    vector<Node> nodes;
    for (int i = 0; i < size; ++i) {
        auto random_x = static_cast<double>(rand() % 100);
        auto random_y = static_cast<double>(rand() % 100);
        nodes.emplace_back(i, random_x, random_y);
    }
    return Chromosome(Graph(nodes));
}

int main(const int argc, char *argv[]) {
    try {
        Chromosome parent1(Graph({
            Node(0, 0.0, 0.0),
            Node(1, 1.0, 1.0),
            Node(2, 6.0, 0.0),
            Node(3, 8.0, 1.0),
        }));

        Chromosome parent2(Graph({
            Node(0, 0.0, 0.0),
            Node(1, 2.0, 2.0),
            Node(2, 5.0, 1.0),
            Node(3, 7.0, 3.0),
        }));

        auto level = Logger::Level::DEBUG;
        vector<Chromosome> chromosomes = {parent1, parent2};
        unique_ptr<SelectionOp> selection = make_unique<LinearRankingSelection>(level, 2);
        unique_ptr<CrossoverOp> crossover = make_unique<OrderCrossover>(level);
        unique_ptr<MutationOp> mutation = make_unique<DisplacementMutation>(level, 0.01);
        unique_ptr<Replacement> replacement = make_unique<SteadyStateReplacement>(level);
        unique_ptr<StoppingCriterion> stopping = make_unique<TimeLimitCriterion>(level, 10);

        GeneticAlgorithm ga(
            chromosomes,
            selection,
            crossover,
            mutation,
            replacement,
            stopping
        );

        ga.start();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
