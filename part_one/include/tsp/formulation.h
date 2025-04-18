#ifndef FORMULATION_H
#define FORMULATION_H
#include "cpxmacro.h"
#include "graph/graph.h"
#include "utils/time_logger.h"

/**
 * Represents a formulation for the TSP problem.
 */
class Formulation {
protected:
    static constexpr int NAME_SIZE = 512;
    Graph graph;
    TimeLogger tl;
    Env env;
    Prob lp;

    virtual void print_solution() const;

public:
    explicit Formulation(const string &graph_file) : graph(graph_file) {
    }

    virtual ~Formulation();

    virtual void setup() = 0;

    virtual void solve() = 0;
};

#endif //FORMULATION_H
