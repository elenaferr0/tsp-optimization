#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>

#include "tsp/graph.h"
#include "tsp/node.h"

using namespace std;

class Chromosome {
protected:
    Graph graph;

public:
    explicit Chromosome(Graph graph);

    double evaluate_fitness() const;

    bool operator<(const Chromosome &other) const;

    size_t get_n_genes() const;

    vector<Node> get_subpath(int start, int end) const;

    void set_subpath(int start, int end, vector<Node> path);

    Node get_node(int i) const;
};

#endif //CHROMOSOME_H
