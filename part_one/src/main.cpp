#include <iostream>
#include <string>
#include "cpxmacro.h"
#include "tsp/gavish_graves_formulation.h"
#include "tsp/miller_tucker_zemlin_formulation.h"

using namespace std;

void parse_params(const int argc, char *argv[], int &timeout, char *&instance, string &formulation) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <instance> [formulation] [timeout]" << std::endl;
        std::cerr << "  formulation: gg (Gavish-Graves) or mtz (Miller-Tucker-Zemlin), defaults to gg" << std::endl;
        exit(1);
    }

    instance = argv[1];
    formulation = "gg"; // Default to Gavish-Graves
    timeout = 0;

    // Check if there are additional arguments
    if (argc > 2) {
        // Check if second argument is a formulation type
        string arg2 = argv[2];
        if (arg2 == "gg" || arg2 == "mtz") {
            formulation = arg2;
            // If there's a third argument, it's the timeout
            if (argc > 3) {
                try {
                    timeout = std::stoi(argv[3]);
                } catch (std::invalid_argument &e) {
                    std::cerr << "Invalid timeout value: " << argv[3] << std::endl;
                    exit(1);
                }
            }
        } else {
            // Second argument is timeout
            try {
                timeout = std::stoi(argv[2]);
            } catch (std::invalid_argument &e) {
                std::cerr << "Invalid argument: " << argv[2] << std::endl;
                std::cerr << "Expected formulation (gg/mtz) or timeout value" << std::endl;
                exit(1);
            }
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
        string formulation;
        parse_params(argc, argv, timeout, instance, formulation);

        if (formulation == "gg") {
            GavishGravesFormulation gg(instance, timeout);
            gg.setup();
            gg.solve();
        } else if (formulation == "mtz") {
            MillerTuckerZemlinFormulation mtz(instance, timeout);
            mtz.setup();
            mtz.solve();
        }
    } catch (std::exception &e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
