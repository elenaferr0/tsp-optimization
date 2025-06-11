#ifndef POPULATION_INITIALIZATION_H
#define POPULATION_INITIALIZATION_H

#include "genetic_algorithm/chromosome/chromosome.h"
#include "utils/logger.h"

class PopulationInitialization {
protected:
  int population_size;
  Logger log;
  Graph graph;
  virtual Chromosome generate_chromosome() = 0;

public:
  PopulationInitialization(Logger::Level log_level, Graph graph, int population_size);
  virtual vector<Chromosome> generate_population() = 0;
  virtual ~PopulationInitialization() = default;
};

#endif // POPULATION_INITIALIZATION_H
