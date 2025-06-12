#ifndef MATHS_H
#define MATHS_H

#include "tsp/node.h"
#include <cmath>
#include <random>
#include <utility>

using namespace std;

static random_device rd;
static mt19937 gen(rd());


inline double euclidean_dist(const pair<double, double> &p1,
                             const pair<double, double> &p2) {
    return sqrt(pow(p1.first - p2.first, 2) + pow(p1.second - p2.second, 2));
}

inline double unif_real(const double a, const double b) {
    if (a > b) {
        throw invalid_argument("Invalid range: a should be less than or equal to b");
    }
    std::uniform_real_distribution<> dis(a, nextafter(b, std::numeric_limits<double>::max()));
    return dis(gen);
}

inline int unif_int(const int a, const int b) {
    if (a > b) {
        throw invalid_argument("Invalid range: a should be less than or equal to b");
    }
    std::uniform_int_distribution<> dis(a, b);
    return dis(gen);
}


// Cross product of vectors OA and OB
// Returns > 0 for counter-clockwise turn, < 0 for clockwise, 0 for collinear
inline double cross_product(const Node &O, const Node &a, const Node &b) {
    return (a.x() - O.x()) * (b.y() - O.y()) - (a.y() - O.y()) * (b.x() - O.x());
}

inline double polar_angle(const pair<double, double> &ref,
                          const pair<double, double> &p) {
    return atan2(p.second - ref.second, p.first - ref.first);
}

inline bool eq(double a, double b, const double epsilon = 1e-9) {
    return abs(a - b) < epsilon;
}

// Comparator for sorting points by polar angle
// inline bool polar_compare(const Node &pivot, const Node &a, const Node &b) {
//     double angle_a = polar_angle(pivot.position, a.position);
//     double angle_b = polar_angle(pivot.position, b.position);
//
//     if (eq(angle_a, angle_b)) {
//         return euclidean_dist(pivot.position, a.position) <
//                euclidean_dist(pivot.position, b.position);
//     }
//     return angle_a < angle_b;
// }

#endif // MATHS_H
