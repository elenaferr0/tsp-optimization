#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <vector>
#include <queue>

using namespace std;

typedef vector<vector<double> > dbl_mat;
template<typename T> using priority_queue_asc = priority_queue<T, vector<T>, greater<T>>;

static auto by_fitness_desc = [](const pair<int, double> &a, const pair<int, double> &b) {
    return a.second < b.second; // Descending order by fitness
};

typedef priority_queue<pair<int, double>, vector<pair<int, double> >, decltype(by_fitness_desc)> priority_queue_by_fitness;

#endif //TYPEDEFS_H
