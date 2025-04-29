#include "../../include/tsp/graph.h"
#include <iostream>

#include "../../include/utils/measurements.h"

using namespace std;

Graph::Graph(const string &file_path) {
    fstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + file_path);
    }

    // Read nodes
    {
        file >> n_nodes;

        for (int i = 0; i < n_nodes; ++i) {
            Node node;
            double x, y;
            file >> node.id >> x >> y;
            node.position = {x, y};
            nodes.push_back(node);
        }
    }

    compute_costs();
    file.close();
}

void Graph::compute_costs() {
    costs = vector<vector<double> >(n_nodes, vector<double>(n_nodes, -1));

    for (int i = 0; i < n_nodes; ++i) {
        for (int j = 0; j < n_nodes; ++j) {
            if (i == j) {
                costs[i][j] = 0;
                continue;
            }

            if (costs[j][i] != -1) // Assumption: cost is always positive and i->j has the same cost as j->i
            {
                costs[i][j] = costs[j][i];
                continue;
            }

            costs[i][j] = euclidean_dist(nodes[i].position, nodes[j].position);
        }
    }
}
