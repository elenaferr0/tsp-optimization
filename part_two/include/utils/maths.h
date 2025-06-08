#ifndef MATHS_H
#define MATHS_H

#include <cmath>
#include <utility>
#include <random>

using namespace std;

inline double euclidean_dist(const pair<double, double>& p1, const pair<double, double>& p2)
{
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

inline double unif(const double a, const double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(a, b);
    return dis(gen);
}

#endif //MATHS_H
