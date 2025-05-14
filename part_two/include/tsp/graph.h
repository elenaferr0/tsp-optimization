#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <vector>
#include "../utils/typedefs.h"

using namespace std;


struct Node {
    int id;
    pair<double, double> position;
};

class Graph {
    vector<Node> nodes;

    void compute_costs();

    explicit Graph(const string &file_path);

public:
    int n_nodes;
    DblMat costs;

    static Graph of_instance(const char *instance_name) {
        const auto path = "./samples/" + string(instance_name) + ".dat";
        return Graph(path);
    }

    double get_cost(int i, int j) const;
};

#endif //GRAPH_H
