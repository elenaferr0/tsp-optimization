#ifndef GRAPH_H
#define GRAPH_H

#include "tsp/node.h"
#include "utils/typedefs.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Graph {
    DblMat costs;
    size_t n_nodes;
    string file_path;

    void compute_costs();

public:
    vector<Node> path; // Public just to allow easy access

    explicit Graph(const string &file_path);

    explicit Graph(const vector<Node> &nodes);

    double get_cost(int i, int j) const;

    Node operator[](int i) const;

    string get_file_path() const;
};

#endif // GRAPH_H
