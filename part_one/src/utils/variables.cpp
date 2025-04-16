#include "utils/variables.h"

void Variables::
    add_var(double cost, double lower_bound, double upper_bound, char type, char *name)
{
    n_vars++;

    costs.push_back(cost);
}

Variables::Variables() : n_vars(0),
                         costs(vector<double>()),
                         types(vector<char>()),
                         names(vector<char *>()),
                         lower_bounds(vector<double>()),
                         upper_bounds(vector<double>())
{
}

void Variables::add_binary_var(double cost, char *name) {
    add_var(cost, 0, 1, 'B', name);
}

void Variables::add_continuous_var(double cost, double lower_bound, double upper_bound, char* name) {
    add_var(cost, lower_bound, upper_bound, 'C', name);
}

void Variables::add_continuous_positive_var(double cost, char *name) {
    add_var(cost, 0, CPX_INFBOUND, 'C', name);
}

void Variables::add_continuous_unbounded_var(double cost, char *name) {
    add_var(cost, CPX_INFBOUND, CPX_INFBOUND, 'C', name);
}