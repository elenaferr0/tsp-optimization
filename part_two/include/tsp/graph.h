#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <vector>
#include "utils/typedefs.h"
#include "tsp/node.h"

using namespace std;


class Graph {
    dbl_mat costs;
    size_t n_nodes;

    void compute_costs();

    explicit Graph(const string &file_path);


public:
    vector<Node> path; // Public just to allow easy access

    static Graph of_instance(const char *instance_name) {
        const auto path = "./samples/" + string(instance_name) + ".dat";
        return Graph(path);
    }

    explicit Graph(const vector<Node> &nodes);

    static Graph empty() {
        return Graph(vector<Node>());
    }

    double get_cost(int i, int j) const;

    Node operator[](int i) const;
};

#endif //GRAPH_H
