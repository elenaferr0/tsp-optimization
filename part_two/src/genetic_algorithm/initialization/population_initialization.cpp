#include "genetic_algorithm/initialization/population_initialization.h"

PopulationInitialization::PopulationInitialization(const Logger::Level log_level, const Graph& graph)
    : log(log_level), graph(graph) {}