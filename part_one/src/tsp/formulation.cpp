#include "tsp/formulation.h"

void Formulation::setup() {
    CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, 10);
    CHECKED_CPX_CALL(CPXsetintparam, env, CPX_PARAM_WRITELEVEL, CPX_WRITELEVEL_NONZEROVARS);
}

void Formulation::print_solution() const {
    int status = CPXgetstat(env, lp);
    const auto status_str = new char[512];
    CPXgetstatstring(env, status, &status_str[0]);
    cout << "Status: " << status_str << endl;

    double objval;
    CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
    cout << "Objective fun value: " << objval << endl;

    delete [] status_str;
}

Formulation::Formulation(const char *instance_name, const string &graph_file)
    : graph(Graph(graph_file)),
      tl(instance_name),
      instance_name(instance_name),
      status(0),
      errmsg{} {
    DECL_ENV(const env);
    this->env = env;
    DECL_PROB(env, const lp, instance_name);
    this->lp = lp;
    CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, 10);
    CHECKED_CPX_CALL(CPXsetintparam, env, CPX_PARAM_WRITELEVEL, CPX_WRITELEVEL_NONZEROVARS);
}

Formulation::~Formulation() {
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    delete errmsg;
}
