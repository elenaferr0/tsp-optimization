#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <vector>
#include "../utils/typedefs.h"

using namespace std;


struct Node {
    string id;
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
};

#endif //GRAPH_H
