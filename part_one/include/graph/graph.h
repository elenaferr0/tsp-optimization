#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <typedefs.h>

using namespace std;


struct Node
{
    string id;
    pair<double, double> position;
};

class Graph
{
    vector<Node> nodes;
    BitMat adjacency_matrix;
    void compute_costs();
public:
    explicit Graph(const string& file_path);
    int n_nodes;
    DblMat costs;
};

#endif //GRAPH_H
