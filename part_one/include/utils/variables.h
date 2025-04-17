#include <vector>

using namespace std;

class Variables
{
    int n_vars;
    vector<double> costs;
    vector<char> types;
    vector<char *> names;
    vector<double> lower_bounds;
    vector<double> upper_bounds;

    void add_var(double cost, double lower_bound, double upper_bound, char type, char *name);

public:
    Variables();

    // Utility methods
    void add_binary_var(double cost, char *name);
    void add_continuous_var(double cost, double lower_bound, double upper_bound, char *name);
    void add_continuous_positive_var(double cost, char *name);
    void add_continuous_unbounded_var(double cost, char *name);

    int get_n_vars() const { return n_vars; }
    double *get_costs() { return costs.data(); }
    char *get_types() { return &types[0]; }
    char **get_names() { return &names[0]; }
    double *get_lower_bounds() { return &lower_bounds[0]; }
    double *get_upper_bounds() { return &upper_bounds[0]; }
};
