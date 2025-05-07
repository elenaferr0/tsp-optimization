#include <iostream>
#include "cpxmacro.h"
#include "../include/tsp/graph.h"
#include "tsp/gavish_graves_formulation.h"
#include "tsp/miller_tucker_zemlin_formulation.h"

using namespace std;

void parse_params(const int argc, char *argv[], int &timeout, char *&instance) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <instance> [timeout]" << std::endl;
        exit(1);
    }

    instance = argv[1];
    timeout = 0;

    if (argc > 2) {
        try {
            timeout = std::stoi(argv[2]);
        } catch (std::invalid_argument &e) {
            std::cerr << "Invalid timeout value: " << argv[2] << std::endl;
            exit(1);
        }
    }
    if (timeout < 0) {
        std::cerr << "Timeout must be a non-negative integer." << std::endl;
        exit(1);
    }
}

int main(const int argc, char *argv[]) {
    try {
        int timeout;
        char *instance = nullptr;
        parse_params(argc, argv, timeout, instance);

        GavishGravesFormulation gg(instance, timeout);
        gg.setup();
        gg.solve();
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
