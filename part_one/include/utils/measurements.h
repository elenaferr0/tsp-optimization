#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <cmath>
#include <utility>

#include "constants.h"

using namespace std;

inline double euclidean_dist(const pair<double, double>& p1, const pair<double, double>& p2)
{
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

#endif //MEASUREMENTS_H
