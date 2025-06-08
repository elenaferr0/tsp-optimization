#include <iostream>
#include "tsp/graph.h"
#include <genetic_algorithm/crossover/order_crossover.h>

using namespace std;

Chromosome create_chromosome(int size, int min_id = 0)
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
        auto chromo1 = create_chromosome(5);
        cout << "Created Chromosome1: " << chromo1 << endl;
        auto chromo2 = create_chromosome(5, 5); // Create a second chromosome with different IDs
        cout << "Created Chromosome2: " << chromo2 << endl;

        OrderCrossover oc;
        oc.recombine({chromo1, chromo2});
    }
    catch (std::exception &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
