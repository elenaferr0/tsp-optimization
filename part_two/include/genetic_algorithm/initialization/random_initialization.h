#ifndef RANDOM_INITIALIZATION_H
#define RANDOM_INITIALIZATION_H

#include <random>
#include <vector>
#include "population_initialization.h"

class RandomInitialization final : public PopulationInitialization {
public:
    RandomInitialization(Logger::Level log_level, const Graph& graph, const HyperParams& params);

    vector<Chromosome> generate_population() override;
};

#endif // RANDOM_INITIALIZATION_H
