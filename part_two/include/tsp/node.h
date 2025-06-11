#ifndef NODE_H
#define NODE_H

#include <utility>

using namespace std;

struct Node {
  int id;
  pair<double, double> position;

  Node(const int id, const double x, const double y)
      : id(id), position(position.first, position.second) {}

  Node() = default;

  bool operator<(const Node &other) const {
    if (position.second != other.position.second) {
      return position.second < other.position.second;
    }
    return position.first < other.position.first;
  }

  double x() const { return position.first; }

  double y() const { return position.second; }
};
#endif // NODE_H
