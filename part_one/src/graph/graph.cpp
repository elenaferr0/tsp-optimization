#include "graph/graph.h"
#include <iostream>

#include "utils/measurements.h"

using namespace std;

Graph::Graph(const string &file_path) {
    fstream file(file_path);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + file_path);
    } {
        // Read nodes
        file >> n_nodes;

        for (int i = 0; i < n_nodes; ++i) {
            Node node;
            double x, y;
            file >> node.id >> x >> y;
            node.position = {x, y};
            nodes.push_back(node);
        }
        adjacency_matrix.resize(n_nodes, vector<bool>(n_nodes, false));
    } {
        // Read edges
        int n_edges;
        file >> n_edges;
        for (int i = 0; i < n_edges; ++i) {
            int from, to;
            file >> from >> to;
            adjacency_matrix[from][to] = true;
            adjacency_matrix[to][from] = true; // Assuming undirected graph
        }
    }

    compute_costs();
    file.close();
}

void Graph::compute_costs() {
    costs = vector<vector<double> >(n_nodes, vector<double>(n_nodes, -1));

    DblMat mem(n_nodes, vector<double>(n_nodes, -1)); // memoize distance computation
    for (int i = 0; i < n_nodes; ++i) {
        for (int j = 0; j < n_nodes; ++j) {
            if (!adjacency_matrix[i][j]) {
                costs[i][j] = 0;
                mem[i][j] = 0;
                continue;
            }

            if (mem[j][i] != -1) // Assumption: cost is always positive and i->j has the same cost as j->i
            {
                costs[i][j] = costs[j][i];
                continue;
            }

            costs[i][j] = euclidean_dist(nodes[i].position, nodes[j].position);
            mem[i][j] = costs[i][j];
        }
    }
}
