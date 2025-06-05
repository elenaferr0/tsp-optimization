#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <vector>
#include "utils/typedefs.h"
#include "tsp/node.h"

using namespace std;


class Graph {
    DblMat costs;
    size_t n_nodes;

    void compute_costs();

    explicit Graph(const string &file_path);

    explicit Graph(const vector<Node> &nodes);

public:
    vector<Node> path; // Public just to allow easy access

    static Graph of_instance(const char *instance_name) {
        const auto path = "./samples/" + string(instance_name) + ".dat";
        return Graph(path);
    }

    static Graph empty() {
        return Graph(vector<Node>());
    }

    double get_cost(int i, int j) const;

    Node operator[](int i) const;
};

#endif //GRAPH_H
