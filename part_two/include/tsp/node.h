#ifndef NODE_H
#define NODE_H

#include <utility>

using namespace std;

struct Node {
    int id;
    pair<double, double> position;

    Node(const int id, const double x, const double y) : id(id), position(x, y) {
    }

    Node() = default;
};
#endif //NODE_H
