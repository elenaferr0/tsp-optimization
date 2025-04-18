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
    string instance_name;
    int status;
    char *errmsg;

    virtual void print_solution() const;
    virtual void export_solution();

public:
    explicit Formulation(const char *instance_name);

    virtual ~Formulation();

    virtual void setup() = 0;

    virtual void solve() = 0;
};

#endif //FORMULATION_H
