#include <iostream>
#include "tsp/graph.h"
#include <genetic_algorithm/replacement/steady_state_replacement.h>

using namespace std;

int main(const int argc, char *argv[]) {
    try {
        SteadyStateReplacement replacement;
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
