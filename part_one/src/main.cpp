#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#include "cpxmacro.h"
#include "utils/time_logger.h"
#include "graph/graph.h"
#include "utils/constraints.h"
#include "utils/variables.h"

using namespace std;

int status;
char errmsg[BUF_SIZE];
constexpr int starting_node = 0;

const int NAME_SIZE = 512;
char name[NAME_SIZE];

// maps
vector<vector<int> > map_x; // x_ij ---> map_x[i][j]
vector<vector<int> > map_y; // y_ij ---> map_y[i][j]


void setup_lp(const CEnv env, const Prob lp, const Graph &graph) {
    const int N = graph.n_nodes;
    int var_pos = 0;
    map_x = vector<vector<int> >(N, vector<int>(N, -1));
    map_y = vector<vector<int> >(N, vector<int>(N, -1));
    Variables vars;
    TimeLogger tl;

    //// Objective function
    // (9) y_ij: min sum_{(i,j) in A} c_ij y_ij
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            snprintf(name, NAME_SIZE, "y_%d_%d", i, j);
            vars.add_binary_var(graph.costs[i][j], name);
            map_y[i][j] = var_pos++;
        }
    }

    // (9) x_ij (does not appear in the objective function)
    for (int i = 0; i < N; ++i) {
        for (int j = 1; j < N; ++j) {
            if (i == j) continue;
            snprintf(name, NAME_SIZE, "x_%d_%d", i, j);
            vars.add_continuous_positive_var(0, name);
            map_x[i][j] = var_pos++;
        }
    }
    CHECKED_CPX_CALL(CPXnewcols, env, lp, vars.get_n_vars(), vars.get_costs(), vars.get_lower_bounds(),
                     vars.get_upper_bounds(), vars.get_types(), vars.get_names());
    tl.tick("Variable creation");

    Constraints ct;
    //// Constraints
    // (10) forall k in N\{0} sum_{i: (i, k) in A} x_ik - sum_{j: (k, j) in A} x_kj = 1
    for (int k = 1; k < N; ++k) {
        ct.new_eq(1.0);
        for (int i = 0; i < N; ++i) {
            if (i == k || map_x[i][k] < 0) continue;
            ct.add_term(map_x[i][k], 1.0);
        }
        for (int j = 0; j < N; ++j) {
            if (j == k || map_x[k][j] < 0) continue;
            ct.add_term(map_x[k][j], -1.0);
        }
    }

    // (11) forall i in N: sum_{j: (i, j) in A} y_ij = 1
    for (int i = 0; i < N; ++i) {
        ct.new_eq(1.0);
        for (int j = 0; j < N; ++j) {
            if (i == j || map_y[i][j] < 0) continue;
            ct.add_term(map_y[i][j], 1.0);
        }
    }

    // (12) forall j in N: sum_{i: (i, j) in A} y_ij = 1
    for (int j = 0; j < N; ++j) {
        ct.new_eq(1.0);
        for (int i = 0; i < N; ++i) {
            if (j == i || map_y[i][j] < 0) continue;
            ct.add_term(map_y[i][j], 1.0);
        }
    }

    // (13) forall (i, j) in A: x_ij - (|N| - 1) y_ij <= 0
    for (int i = 0; i < N; ++i) {
        for (int j = 1; j < N; ++j) {
            if (i == j || map_x[i][j] < 0 || map_y[i][j] < 0) continue;
            ct.new_le(0.0);
            ct.add_term(map_x[i][j], 1.0);
            ct.add_term(map_y[i][j], -(N - 1.0));
        }
    }

    // Add all constraints at once
    CHECKED_CPX_CALL(
        CPXaddrows,
        env,
        lp,
        0,
        ct.get_n_rows(),
        ct.get_nzcnt(),
        ct.get_rhs(),
        ct.get_senses(),
        ct.get_matbeg(),
        ct.get_indexes(),
        ct.get_coefficients(),
        nullptr,
        nullptr
    );
    tl.tick("Constraint creation");
}

void parse_args(int argc, char const *argv[], double &timeout, string &graph_file) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <timeout> <graph_path>" << endl;
        exit(1);
    }

    // timeout = atof(argv[1]);
    // if (timeout <= 0) {
    //     cout << "Invalid timeout value: " << timeout << endl;
    //     exit(1);
    // }

    graph_file = argv[2];
    if (graph_file.empty()) {
        cout << "Invalid graph file path: " << graph_file << endl;
        exit(1);
    }
}

void print_sln(const Env &env, const Prob &lp, const Graph &graph) {
    status = CPXgetstat(env, lp);
    auto status_string = new char[512];
    CPXgetstatstring(env, status, &status_string[0]);
    cout << "Status: " << status_string << endl;

    double objval;
    CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
    cout << "Objective fun value: " << objval << endl;

    delete [] status_string;
}

int main(const int argc, char const *argv[]) {
    double timeout = 10;
    string graph_file;
    parse_args(argc, argv, timeout, graph_file);

    Graph graph(graph_file);

    try {
        DECL_ENV(env);
        DECL_PROB(env, lp);
        CHECKED_CPX_CALL(CPXsetdblparam, env, CPX_PARAM_TILIM, 10);
        CHECKED_CPX_CALL(CPXsetintparam, env, CPX_PARAM_WRITELEVEL, CPX_WRITELEVEL_NONZEROVARS);

        TimeLogger tl;
        setup_lp(env, lp, graph);
        tl.tick("Model setup");
        CHECKED_CPX_CALL(CPXmipopt, env, lp);
        tl.tick("MIP optimization");

        print_sln(env, lp, graph);

        CHECKED_CPX_CALL(CPXsolwrite, env, lp, "tsp.sol");
        tl.tick("Solution write");
        tl.log_total_time("Total time");

        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);
    } catch (std::exception &e) {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
