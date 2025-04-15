#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

typedef vector<vector<bool>> bit_matrix;
typedef vector<vector<double>> matrix;

struct Node
{
    string id;
    pair<double, double> position;
};

class Graph
{
    vector<Node> nodes;
    bit_matrix adjacency_matrix;
    void compute_costs();
public:
    explicit Graph(const string& file_path);
    int n_nodes;
    matrix costs;
};

#endif //GRAPH_H
