#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#include "cpxmacro.h"
#include "utils/time_logger.h"
#include "graph/graph.h"

using namespace std;

int status;
char errmsg[BUF_SIZE];
constexpr int starting_node = 0;

const int NAME_SIZE = 512;
char name[NAME_SIZE];

// maps
vector<vector<int>> map_x; // x_ij ---> map_x[i][j]
vector<vector<int>> map_y; // y_ij ---> map_y[i][j]


void setup_lp(const CEnv env, const Prob lp, const Graph& graph)
{
    int N = graph.n_nodes;

    int var_pos = 0;
    // Initialize map_x and map_y with -1
    map_x = vector<vector<int>>(N, vector<int>(N, -1));
    map_y = vector<vector<int>>(N, vector<int>(N, -1));

    //// Objective function
    // (9) y_ij: min sum_{(i,j) in A} c_ij y_ij
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (i == j)
                continue;
            constexpr auto ytype = CPX_BINARY;
            snprintf(name, NAME_SIZE, "y_%d_%d", i, j);
            auto yname = &name[0];

            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &graph.costs[i][j], nullptr, nullptr, &ytype, &yname);
            map_y[i][j] = var_pos++;
        }
    }

    // (9) x_ij (does not appear in the objective function)
    for (int i = 0; i < N; ++i)
    {
        for (int j = 1; j < N; ++j) // No need to send flow towards node 0
        {
            if (i == j)
                continue;
            constexpr auto xtype = CPX_CONTINUOUS;
            snprintf(name, NAME_SIZE, "x_%d_%d", i, j);
            auto xname = &name[0];
            constexpr auto zero = 0.0;
            constexpr double lb = 0, ub = CPX_INFBOUND;

            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &zero, &lb, &ub, &xtype, &xname);
            map_x[i][j] = var_pos++;
        }
    }

    //// Constraints
    // (10) forall k in N\{0} sum_{i: (i, k) in A} x_ik - sum_{j: (k, j) in A} x_kj = 1
    {
        for (int k = 1; k < N; ++k)
        {
            vector<int> idx;
            vector<double> coef;

            for (int i = 0; i < N; ++i)
            {
                if (i == k || map_x[i][k] < 0)
                    continue;
                idx.push_back(map_x[i][k]);
                coef.push_back(1.0);
            }

            for (int j = 0; j < N; ++j)
            {
                if (j == k || map_x[k][j] < 0)
                    continue;
                idx.push_back(map_x[k][j]);
                coef.push_back(-1.0);
            }

            auto sense = 'E';
            auto rhs = 1.0;
            auto matbeg = 0;
            CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], nullptr,
                             nullptr);
        }
    }

    // (11) forall i in N: sum_{j: (i, j) in A} y_ij = 1
    for (int i = 0; i < N; ++i)
    {
        vector<int> idx;
        vector<double> coef;

        for (int j = 0; j < N; ++j)
        {
            if (i == j || map_y[i][j] < 0)
                continue;

            idx.push_back(map_y[i][j]);
            coef.push_back(1.0);
        }

        auto sense = 'E';
        auto rhs = 1.0;
        auto matbeg = 0;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], nullptr,
                         nullptr);
    }

    // (12) forall j in N: sum_{i: (i, j) in A} y_ij = 1
    for (int j = 0; j < N; ++j)
    {
        vector<int> idx;
        vector<double> coef;

        for (int i = 0; i < N; ++i)
        {
            if (j == i || map_y[i][j] < 0)
                continue;

            idx.push_back(map_y[i][j]);
            coef.push_back(1.0);
        }

        auto sense = 'E';
        auto rhs = 1.0;
        auto matbeg = 0;
        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], nullptr,
                         nullptr);
    }

    // (13) forall (i, j) in A: x_ij - (|N| - 1) y_ij <= 0
    for (int i = 0; i < N; ++i)
    {
        for (int j = 1; j < N; ++j) // x_i0 = 0 for all i
        {
            if (i == j || map_x[i][j] < 0 || map_y[i][j] < 0)
                continue;
            vector<int> idx(2);
            vector<double> coef(2);
            auto sense = 'L';

            idx[0] = map_x[i][j]; // x_ij
            idx[1] = map_y[i][j]; // y_ij
            coef[0] = 1.0;
            coef[1] = -(N - 1.0);

            constexpr auto zero = 0.0;
            constexpr auto matbeg = 0;

            CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &zero, &sense, &matbeg, &idx[0], &coef[0], nullptr,
                             nullptr);
        }
    }
}

void parse_args(int argc, char const* argv[], double& timeout, int& size)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <timeout> <size>" << endl;
        exit(1);
    }

    timeout = atof(argv[1]);
    if (timeout <= 0)
    {
        cout << "Invalid timeout value: " << timeout << endl;
        exit(1);
    }

    size = atoi(argv[2]);
    if (size <= 0)
    {
        cout << "Invalid size value: " << size << endl;
        exit(1);
    }
}

void print_sln(const Env& env, const Prob& lp, const Graph& graph)
{
    int N = graph.n_nodes;
    // Print solution status
    status = CPXgetstat(env, lp);
    if (status == CPXMIP_OPTIMAL)
        cout << "Solution status: Optimal" << endl;
    else if (status == CPXMIP_INFEASIBLE)
        cout << "Solution status: Infeasible" << endl;
    else if (status == CPXMIP_UNBOUNDED)
        cout << "Solution status: Unbounded" << endl;
    else
        cout << "Solution status: Unknown" << endl;


    double objval;
    CHECKED_CPX_CALL(CPXgetobjval, env, lp, &objval);
    cout << "Objective fun value: " << objval << endl;

    const auto n = CPXgetnumcols(env, lp);
    vector<double> variables(n);
    CHECKED_CPX_CALL(CPXgetx, env, lp, &variables[0], 0, n - 1);

    ostringstream buffer_x;
    ostringstream buffer_y;
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (map_x[i][j] >= 0)
                buffer_x << "x_" << i << j << ": " << variables[map_x[i][j]] << endl;

            if (map_y[i][j] >= 0)
                buffer_y << "y_" << i << j << ": " << variables[map_y[i][j]] << endl;
        }
    }

    cout << "x variables:\n"
        << buffer_x.str() << endl;
    cout << "y variables:\n"
        << buffer_y.str() << endl;
}

int main(const int argc, char const* argv[])
{
    Graph graph("samples/graph.txt");

    // double timeout = 1;
    // int size = 0;
    //
    // parse_args(argc, argv, timeout, size);

    try
    {
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
    }
    catch (std::exception& e)
    {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
