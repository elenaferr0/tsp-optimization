#ifndef GRAPH_H
#define GRAPH_H

#include "tsp/node.h"
#include "utils/typedefs.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Graph {
  dbl_mat costs;
  size_t n_nodes;
  string file_path;

  void compute_costs();

  explicit Graph(const string &file_path);

public:
  vector<Node> path; // Public just to allow easy access

  static Graph from_file(const char *instance_name) {
    const auto path = "./samples/" + string(instance_name) + ".dat";
    return Graph(path);
  }

  explicit Graph(const vector<Node> &nodes);

  double get_cost(int i, int j) const;

  Node operator[](int i) const;

  string get_file_path() const;
  
};

#endif // GRAPH_H
