#include "genetic_algorithm/chromosome/tsp_path_chromosome.h"

TSPPathChromosome::TSPPathChromosome(Graph graph) : Chromosome(), graph(graph) {

}

double TSPPathChromosome::evaluate_fitness() const {
    double fitness = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        fitness += graph.get_cost(path[i].id, path[i + 1].id);
    }
    fitness += graph.get_cost(path.back().id, path.front().id); // Return to the starting point
    return fitness;
}