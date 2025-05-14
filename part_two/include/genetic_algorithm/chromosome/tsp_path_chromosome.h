#ifndef TSP_SOLUTION_ENCODER_H
#define TSP_SOLUTION_ENCODER_H
#include <vector>

#include "chromosome.h"
#include "tsp/graph.h"

using namespace std;

class TSPPathChromosome final : public Chromosome {
    Graph graph;
    vector<Node> path;

public:
    explicit TSPPathChromosome(Graph graph);
    double evaluate_fitness() const override;
};

#endif //TSP_SOLUTION_ENCODER_H
