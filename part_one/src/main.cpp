#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>

#include "cpxmacro.h"
#include "utils/time_logger.h"
#include "graph/graph.h"
#include "tsp/gavish_graves_formulation.h"
#include "utils/constraints.h"
#include "utils/variables.h"

using namespace std;

// char errmsg[BUF_SIZE];
constexpr int starting_node = 0;

void parse_args(int argc, char const *argv[], double &timeout, string &graph_file) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <timeout> <graph_path>" << endl;
        exit(1);
    }

    // timeout = atof(argv[1]);
    // if (timeout <= 0) {
    //     cout << "Invalid timeout value: " << timeout << endl;
    //     exit(1);
    // }

    graph_file = argv[2];
    if (graph_file.empty()) {
        cout << "Invalid graph file path: " << graph_file << endl;
        exit(1);
    }
}

int main(const int argc, char const *argv[]) {
    double timeout = 10;
    string graph_file;
    parse_args(argc, argv, timeout, graph_file);

    try {
        GavishGravesFormulation gg("graph", graph_file );
        gg.setup();
        gg.solve();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
