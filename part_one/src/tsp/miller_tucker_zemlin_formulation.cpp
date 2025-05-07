#include "tsp/miller_tucker_zemlin_formulation.h"

#include "utils/constraints.h"
#include "utils/variables.h"

MillerTuckerZemlinFormulation::MillerTuckerZemlinFormulation(const char *instance_name, int timeout)
    : Formulation(instance_name, timeout) {
    const int N = graph.n_nodes;
    map_x = vector<vector<int> >(N, vector<int>(N, -1));
    map_u = vector<int>(N, -1);
}

void MillerTuckerZemlinFormulation::create_variables() {
    const int N = graph.n_nodes;
    int var_pos = 0;

    Variables vars;
    char name[NAME_SIZE];

    //// Objective function
    //// x_ij: min sum_{(i, j, i != j) in A} c_ij x_ij
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == j) continue;
            snprintf(name, NAME_SIZE, "x_%d_%d", i, j);
            vars.add_binary_var(graph.costs[i][j], name);
            map_x[i][j] = var_pos++;
        }
    }

    // u_i (does not appear in the objective function)
    for (int i = 1; i < N; ++i) {
        snprintf(name, NAME_SIZE, "u_%d", i);
        vars.add_positive_int_var(0, name);
        map_u[i] = var_pos++;
    }

    // Add all variables at once
    CHECKED_CPX_CALL(CPXnewcols, env, lp, vars.get_n_vars(), vars.get_costs(), vars.get_lower_bounds(),
                     vars.get_upper_bounds(), vars.get_types(), vars.get_names());
    tl.tick("Variables creation");
}

void MillerTuckerZemlinFormulation::create_constraints() {
    const int N = graph.n_nodes;
    Constraints ct;

    // forall j in N: sum_{i = 0, i != j}^N x_ij = 1
    for (int j = 0; j < N; ++j) {
        ct.new_eq(1.0);
        for (int i = 0; i < N; ++i) {
            if (i == j || map_x[i][j] < 0) continue;
            ct.add_term(map_x[i][j], 1.0);
        }
    }

    // forall i in N: sum_{j = 0, j != i}^N x_ij = 1
    for (int i = 0; i < N; ++i) {
        ct.new_eq(1.0);
        for (int j = 0; j < N; ++j) {
            if (i == j || map_x[i][j] < 0) continue;
            ct.add_term(map_x[i][j], 1.0);
        }
    }

    // u_i - u_j + N * x_ij <= N - 1
    for (int i = 1; i < N; ++i) {
        for (int j = 1; j < N; ++j) {
            if (i == j || map_x[i][j] < 0 || map_u[i] < 0 || map_u[j] < 0) continue;
            ct.new_le(N - 1);
            ct.add_term(map_u[i], 1.0);
            ct.add_term(map_u[j], -1.0);
            ct.add_term(map_x[i][j], N);
        }
    }

    // u_i >= 1
    for (int i = 1; i < N; ++i) {
        if (map_u[i] < 0) continue;
        ct.new_ge(1.0);
        ct.add_term(map_u[i], 1.0);
    }

    // u_i <= N - 1
    for (int i = 1; i < N; ++i) {
        if (map_u[i] < 0) continue;
        ct.new_le(N - 1);
        ct.add_term(map_u[i], 1.0);
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

    tl.tick("Constraints creation");
}

string MillerTuckerZemlinFormulation::formulation_code() const {
    return "mtz"; // Miller-Tucker-Zemlin
}
