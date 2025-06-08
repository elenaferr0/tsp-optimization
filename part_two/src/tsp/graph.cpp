#include "tsp/graph.h"
#include "utils/maths.h"

using namespace std;

Graph::Graph(const vector<Node>& nodes): n_nodes(nodes.size()), path(nodes) {
    compute_costs();
}

Graph::Graph(const string &file_path) : n_nodes(0)
{
    fstream file(file_path);
    if (!file.is_open())
    {
        throw runtime_error("Could not open file: " + file_path);
    }

    // Read nodes
    {
        file >> n_nodes;

        for (int i = 0; i < n_nodes; ++i)
        {
            Node node;
            double x, y;
            file >> node.id >> x >> y;
            node.position = {x, y};
            path.push_back(node);
        }
    }

    compute_costs();
    file.close();
}

void Graph::compute_costs() {
    costs = vector<vector<double> >(n_nodes, vector<double>(n_nodes, -1));
    // Iterate only bottom triangular matrix
    for (int i = 1; i < n_nodes; ++i) {
        for (int j = 0; j < i; ++j) {
            costs[i][j] = euclidean_dist(path[i].position, path[j].position);
            costs[j][i] = costs[i][j]; // Symmetric
        }
    }
}

double Graph::get_cost(const int x, const int y) const
{
    if (x < 0 || x >= n_nodes || y < 0 || y >= n_nodes)
    {
        throw out_of_range("(get_cost) Node index out of range");
    }
    return costs[x][y];
}

Node Graph::operator[](const int i) const {
    if (i < 0 || i >= n_nodes) {
        throw out_of_range("(operator[]) Node index out of range");
    }
    return path[i];
}
