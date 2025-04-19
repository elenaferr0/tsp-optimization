#include <iostream>
#include "cpxmacro.h"
#include "../include/tsp/graph.h"
#include "tsp/gavish_graves_formulation.h"
#include "tsp/miller_tucker_zemlin_formulation.h"

using namespace std;

int main(const int argc, char const *argv[]) {
    try {
        GavishGravesFormulation gg("cities");
        gg.setup();
        gg.solve();

        MillerTuckerZemlinFormulation mtz("cities");
        mtz.setup();
        mtz.solve();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
