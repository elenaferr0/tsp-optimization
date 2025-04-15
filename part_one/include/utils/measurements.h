#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <cmath>
#include <utility>

#include "constants.h"

using namespace std;

// TODO: could be a strategy
inline double euclidean_dist(const pair<double, double>& p1, const pair<double, double>& p2)
{
    return sqrt(abs(pow(p1.first - p1.second, 2) - pow(p2.first - p2.second, 2)));
}

inline double cm_to_m(const double& cm)
{
    return cm / 100;
}

inline double displacement(const pair<double, double>& p1, const pair<double, double>& p2)
{
    // t = sqrt(s/(0.5 * a))
    const auto s = euclidean_dist(p1, p2);
    return sqrt(cm_to_m(s) / (0.5 * ACCELERATION_SI));
}


#endif //MEASUREMENTS_H
