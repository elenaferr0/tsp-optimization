#include <vector>

using namespace std;

class Variables
{
private:
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
    void add_binary_var(double cost, char* name);
    void add_continuous_var(double cost, double lower_bound, double upper_bound);
    void add_continuous_positive_var(double cost, char* name);
    void add_continuous_unbounded_var(double cost, char* name);
};