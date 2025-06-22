#include "tsp/formulation.h"

void Formulation::setup() {
    tl.start();
    create_variables();
    create_constraints();
    tl.log_total_time("Model setup");
}

void Formulation::solve() {
    CHECKED_CPX_CALL(CPXmipopt, env, lp);
    tl.tick("MIP optimization");
    tl.log_total_time("Exact solution");
    print_solution();
    export_solution();
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

void Formulation::export_solution() {
    const auto path = instance_formulation_code() + ".sol";
    CHECKED_CPX_CALL(CPXsolwrite, env, lp, path.c_str());
}

string Formulation::instance_formulation_code() const {
    return instance_name + "_" + formulation_code();
}

Formulation::Formulation(const char *instance_name, const int timeout)
    : graph(Graph::of_instance(instance_name)),
      tl(instance_name),
      instance_name(instance_name),
      status(0),
      errmsg{} {
    DECL_ENV(const env);
    this->env = env;
    DECL_PROB(env, const lp, instance_name);
    this->lp = lp;
    CHECKED_CPX_CALL(CPXsetintparam, env, CPX_PARAM_WRITELEVEL, CPX_WRITELEVEL_NONZEROVARS);
    if (timeout > 0) {
        CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, timeout);
    }
}

Formulation::Formulation(const Formulation &other) : graph(other.graph), tl(TimeLogger(other.tl)),
                                                     env(other.env), // shallow copy
                                                     lp(other.lp), // shallow copy
                                                     instance_name(other.instance_name),
                                                     status(other.status),
                                                     errmsg(new char[NAME_SIZE]) {
    std::strcpy(errmsg, other.errmsg);
}

Formulation &Formulation::operator=(const Formulation &other) {
    if (this == &other) {
        return *this;
    }
    // free the current resources
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);
    delete[] errmsg;

    graph = other.graph;
    tl = other.tl;
    env = other.env; // shallow copy
    lp = other.lp; // shallow copy
    instance_name = other.instance_name;
    status = other.status;
    errmsg = new char[NAME_SIZE];
    std::strcpy(errmsg, other.errmsg);
    return *this;
}

Formulation::~Formulation() {
    CPXfreeprob(env, &lp);
    CPXcloseCPLEX(&env);

    delete errmsg;
}
