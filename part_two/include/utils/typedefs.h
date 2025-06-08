#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <vector>

using namespace std;

typedef vector<vector<double> > dbl_mat;
template<typename T> using priority_queue_asc = priority_queue<T, vector<T>, greater<T>>;

#endif //TYPEDEFS_H
