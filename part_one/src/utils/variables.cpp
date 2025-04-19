#include "utils/variables.h"

#include <iostream>

#include "cpxmacro.h"

void Variables::
add_var(const double cost, const double lower_bound, const double upper_bound, char type, char *name) {
    n_vars++;

    costs.push_back(cost);
    types.push_back(type);
    lower_bounds.push_back(lower_bound);
    upper_bounds.push_back(upper_bound);
    // Create a copy of name to avoid changes from outside to reflect in the names array
    names.push_back(strdup(name));
}

Variables::Variables() : n_vars(0),
                         costs(vector<double>()),
                         types(vector<char>()),
                         names(vector<char *>()),
                         lower_bounds(vector<double>()),
                         upper_bounds(vector<double>()) {
}

Variables::~Variables() {
    for (const auto n: names) {
        delete n;
    }
    names.clear();
}

void Variables::add_binary_var(const double cost, char *name) {
    add_var(cost, 0, 1, CPX_BINARY, name);
}

void Variables::add_positive_int_var(const double cost, char *name) {
    add_var(cost, 0, CPX_INFBOUND, CPX_INTEGER, name);
}

void Variables::add_continuous_var(const double cost, const double lower_bound, const double upper_bound, char *name) {
    add_var(cost, lower_bound, upper_bound, CPX_CONTINUOUS, name);
}

void Variables::add_continuous_positive_var(const double cost, char *name) {
    add_var(cost, 0, CPX_INFBOUND, CPX_CONTINUOUS, name);
}

void Variables::add_continuous_unbounded_var(const double cost, char *name) {
    add_var(cost, CPX_INFBOUND, CPX_INFBOUND, CPX_CONTINUOUS, name);
}
