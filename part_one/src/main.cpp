#include <iostream>
#include <vector>

#include "cpxmacro.h"

using namespace std;

int status;
char errmsg[BUF_SIZE];

constexpr int N = 3; // Graph nodes
constexpr char node_names[N] = {'A', 'B', 'C'};

// Arcs
constexpr int A[N][N] = {
    {0, 1, 2},
    {0, 2, 3},
    {1, 2, 3}
};

// Time taken by the drill to move from i to j
constexpr double C[N][N] = {
    {0.0, 1.0, 2.0},
    {1.0, 0.0, 3.0},
    {2.0, 3.0, 0.0}
};

constexpr int starting_node = 0;

const int NAME_SIZE = 512;
char name[NAME_SIZE];

// maps
vector<vector<int>> map_x; // x_ij ---> map_x[i][j]
vector<vector<int>> map_y; // y_ij ---> map_y[i][j]

void setup_lp(const CEnv env, const Prob lp)
{
    int var_pos = 0;
    // Initialize map_x and map_y with -1
    map_x = vector<vector<int>>(N, vector<int>(N, -1));
    map_y = vector<vector<int>>(N, vector<int>(N, -1));

    //// Objective function
    // Add y_ij: min sum_{(i,j) in A} c_ij y_ij
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            constexpr auto ytype = CPX_BINARY;
            snprintf(name, NAME_SIZE, "y_%c%c", node_names[i], node_names[j]);
            auto yname = &name[0];

            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &(C[i][j]), nullptr, nullptr, &ytype, &yname);
            map_y[i][j] = var_pos++;
        }
    }

    // Add x_ij (TODO: check if this is needed)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            constexpr auto xtype = CPX_BINARY;
            snprintf(name, NAME_SIZE, "x_%c%c", node_names[i], node_names[j]);
            auto xname = &name[0];
            constexpr auto zero = 0.0;
            constexpr double lb = 0, ub = CPX_INFBOUND;

            CHECKED_CPX_CALL(CPXnewcols, env, lp, 1, &zero, &lb, &ub, &xtype, &xname);
            map_x[i][j] = var_pos++;
        }
    }

    //// Constraints
    {
        vector<int> idx;
        vector<double> coef;
        vector<char*> names;

        for (int j = 0; j < N; j++)
        {
            auto x_0j = map_x[starting_node][j];
            if (x_0j < 0) continue;

            idx.push_back(map_x[starting_node][j]);
            coef.push_back(1.0);
            snprintf(name, NAME_SIZE, "x_0%c", node_names[j]);
            names.push_back(&name[0]);
        }

        constexpr auto sense = 'E';
        constexpr auto rhs = N - 1.0;
        constexpr auto matbeg = 0;

        CHECKED_CPX_CALL(CPXaddrows, env, lp, 0, 1, idx.size(), &rhs, &sense, &matbeg, &idx[0], &coef[0], nullptr,
                         &names[0]);
    }
}

int main(int argc, char const* argv[])
{
    try
    {
        DECL_ENV(env);
        DECL_PROB(env, lp);

        setup_lp(env, lp);
        CPXfreeprob(env, &lp);
        CPXcloseCPLEX(&env);
    }
    catch (std::exception& e)
    {
        std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    }
    return 0;
}
