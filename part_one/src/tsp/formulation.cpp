#include "tsp/formulation.h"

void Formulation::setup() {
    DECL_ENV(const env);
    DECL_PROB(env, lp);
    CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, 10);
    CHECKED_CPX_CALL(CPXsetintparam, env, CPX_PARAM_WRITELEVEL, CPX_WRITELEVEL_NONZEROVARS);
}

void Formulation::print_solution() const {
    int status = CPXgetstat(env, lp);
    auto status_str = new char[512];
    CPXgetstatstring(env, status, &status_str[0]);
    cout << "Status: " << status_str << endl;

    double objval;
    CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
    cout << "Objective fun value: " << objval << endl;

    delete [] status_str;
}

Formulation::~Formulation() {
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);
}
