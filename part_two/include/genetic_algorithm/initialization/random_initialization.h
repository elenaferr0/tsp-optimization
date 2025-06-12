#ifndef RANDOM_INITIALIZATION_H
#define RANDOM_INITIALIZATION_H

#include <random>
#include <vector>
#include "population_initialization.h"

class RandomInitialization final : public PopulationInitialization {
public:
    RandomInitialization(Logger::Level log_level, Graph graph, int population_size);

    vector<Chromosome> generate_population() override;
};

#endif // RANDOM_INITIALIZATION_H
