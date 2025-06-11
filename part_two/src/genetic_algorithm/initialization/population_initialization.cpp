#include "genetic_algorithm/initialization/population_initialization.h"

PopulationInitialization::PopulationInitialization(Logger::Level log_level, Graph graph, int population_size)
    : log(log_level), population_size(population_size), graph(graph) {}