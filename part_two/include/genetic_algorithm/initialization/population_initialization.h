#ifndef POPULATION_INITIALIZATION_H
#define POPULATION_INITIALIZATION_H

#include <memory>
#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/hyper_params.h"
#include "utils/logger.h"

using namespace std;

class PopulationInitialization {
protected:
    const Logger log;
    Graph graph;

public:
    PopulationInitialization(const Logger& log, const Graph &graph);

    virtual vector<Chromosome> generate_population(const HyperParams &params) const = 0;

    virtual unique_ptr<PopulationInitialization> clone() const = 0;

    virtual ~PopulationInitialization() = default;
};

#endif // POPULATION_INITIALIZATION_H
