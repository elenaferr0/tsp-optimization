#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <chrono>
#include <functional>
#include <queue>
#include <vector>

using namespace std;

typedef vector<vector<double>> DblMat;
template <typename T>
using priority_queue_asc = priority_queue<T, vector<T>, greater<T>>;

// Comparator for sorting pairs (index, fitness)
typedef function<bool(const pair<int, double> &, const pair<int, double> &)>
    pair_comp;
typedef priority_queue<pair<int, double>, vector<pair<int, double>>, pair_comp>
    priority_queue_by_fitness;

static auto by_fitness_desc = [](const pair<int, double> &a,
                                 const pair<int, double> &b) {
  return a.second < b.second; // Descending order by fitness
};

static auto by_fitness_asc = [](const pair<int, double> &a,
                                const pair<int, double> &b) {
  return a.second > b.second; // Ascending order by fitness
};

typedef chrono::high_resolution_clock::time_point time_pt;

#endif // TYPEDEFS_H
