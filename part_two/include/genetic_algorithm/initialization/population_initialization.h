#ifndef POPULATION_INITIALIZATION_H
#define POPULATION_INITIALIZATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "genetic_algorithm/hyper_params.h"
#include "utils/logger.h"

class PopulationInitialization {
protected:
  HyperParams params;
  Logger log;
  Graph graph;
public:
  PopulationInitialization(Logger::Level log_level, const Graph& graph, const HyperParams& params);
  virtual vector<Chromosome> generate_population() = 0;
  virtual ~PopulationInitialization() = default;
};

#endif // POPULATION_INITIALIZATION_H
