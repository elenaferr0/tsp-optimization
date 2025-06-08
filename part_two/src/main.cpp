#include <iostream>
#include "tsp/graph.h"
#include <genetic_algorithm/crossover/order_crossover.h>

#include "genetic_algorithm/mutation/displacement_mutation.h"

using namespace std;

Chromosome create_chromosome(const int size, const int min_id = 0)
{
    // Create a simple graph with 'size' nodes
    vector<Node> nodes;
    for (int i = 0; i < size; ++i)
    {
        nodes.emplace_back(i, static_cast<double>(i) + min_id, static_cast<double>(i) + min_id);
    }
    return Chromosome(Graph(nodes));
}

int main(const int argc, char *argv[])
{
    try
    {
        auto chromo1 = create_chromosome(8);
        cout << "Created Chromosome1: " << chromo1 << endl;

        DisplacementMutation dt(1);
        auto out = dt.mutate({chromo1});
        cout << "Mutated Chromosome1: " << out[0] << endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
