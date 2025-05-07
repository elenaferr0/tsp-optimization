#ifndef FORMULATION_H
#define FORMULATION_H
#include "cpxmacro.h"
#include "graph.h"
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

    virtual void create_constraints() = 0;

    virtual void create_variables() = 0;

    /**
     * @return short string representation of the formulation name.
     */
    virtual string formulation_code() const = 0;

    /**
     * @return short string representation of the instance name and its formulation.
     */
    string instance_formulation_code() const;

public:
    explicit Formulation(const char *instance_name, int timeout);

    Formulation(const Formulation &);

    Formulation &operator=(const Formulation &);

    virtual ~Formulation();

    virtual void setup();

    virtual void solve();
};

#endif //FORMULATION_H
