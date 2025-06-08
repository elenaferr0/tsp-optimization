#include <iostream>
#include "tsp/graph.h"
#include <genetic_algorithm/crossover/order_crossover.h>
#include <utils/path.h>

#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "genetic_algorithm/replacement/steady_state_replacement.h"

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
        }));

        Chromosome parent2(Graph({
            Node(0, 2, 0.0),
            Node(1, 10, 1.0),
        }));

        auto parents = vector<Chromosome>{parent1, parent2};
        cout << "Parent 1 Fitness: " << parent1.evaluate_fitness() << endl;
        cout << "Parent 2 Fitness: " << parent2.evaluate_fitness() << endl;

        Chromosome off1(Graph({
            Node(0, 5, 0.0),
            Node(1, 1.0, 1.0),
        }));

        Chromosome off2(Graph({
            Node(0, 4, 0.0),
            Node(1, 10, 1.0),
        }));
        cout << "Offspring 1 Fitness: " << off1.evaluate_fitness() << endl;
        cout << "Offspring 2 Fitness: " << off2.evaluate_fitness() << endl;

        SteadyStateReplacement replacement(1);
        auto new_population = replacement.replace(parents, {off1, off2});
        for (const auto &chromosome : new_population) {
            cout << "New Chromosome Fitness: " << chromosome.evaluate_fitness() << endl;
        }
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
