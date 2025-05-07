#include <iostream>
#include "cpxmacro.h"
#include "../include/tsp/graph.h"
#include "tsp/gavish_graves_formulation.h"
#include "tsp/miller_tucker_zemlin_formulation.h"

using namespace std;

int main() {
    try {
        GavishGravesFormulation gg("pcb_20");
        gg.setup();
        gg.solve();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
