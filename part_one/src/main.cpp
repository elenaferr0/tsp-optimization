#include <iostream>
#include "cpxmacro.h"
#include "graph/graph.h"
#include "tsp/gavish_graves_formulation.h"

using namespace std;

int main(const int argc, char const *argv[]) {
    try {
        GavishGravesFormulation gg("random10");
        gg.setup();
        gg.solve();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
