#include "genetic_algorithm/initialization/population_initialization.h"

PopulationInitialization::PopulationInitialization(const Logger& log, const Graph& graph)
    : log(log), graph(graph) {}