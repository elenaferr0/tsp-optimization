#include "tsp/gavish_graves_formulation.h"

#include "utils/variables.h"
#include "cpxmacro.h"
#include "utils/constraints.h"

GavishGravesFormulation::GavishGravesFormulation(const string &graph_file) : Formulation(graph_file) {
    const int N = graph.n_nodes;
    map_x = vector<vector<int> >(N, vector<int>(N, -1));
    map_y = vector<vector<int> >(N, vector<int>(N, -1));
}


void GavishGravesFormulation::setup() {
    Formulation::setup();
    tl.start();
    create_constraints();
    tl.tick("Constraint creation");
    create_variables();
    tl.tick("Variable creation");
    tl.log_total_time("Model setup");
}

void GavishGravesFormulation::solve() {
    CHECKED_CPX_CALL(CPXmipopt, env, lp);
    tl.tick("MIP optimization");
    print_solution();
    CHECKED_CPX_CALL(CPXsolwrite, env, lp, "tsp.sol");
}

void GavishGravesFormulation::create_constraints() {
    const int N = graph.n_nodes;
    int var_pos = 0;

    Variables vars;
    char name[NAME_SIZE];

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
}

void GavishGravesFormulation::create_variables() const {
    const int N = graph.n_nodes;
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
