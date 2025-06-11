#include "genetic_algorithm/genetic_algorithm.h"

GeneticAlgorithm::GeneticAlgorithm(const vector<Chromosome> &initial_population,
                                   unique_ptr<SelectionOp> &selection,
                                   unique_ptr<CrossoverOp> &crossover,
                                   unique_ptr<MutationOp> &mutation,
                                   unique_ptr<Replacement> &replacement,
                                   unique_ptr<StoppingCriterion> &stopping,
                                   const Logger::Level log_level)
    : population(initial_population), selection(move(selection)),
      crossover(move(crossover)), mutation(move(mutation)),
      replacement(move(replacement)), stopping(move(stopping)), generation(0),
      log(log_level, "GeneticAlgorithm") {
  if (population.empty()) {
    throw runtime_error("Initial population cannot be empty");
  }
}

void GeneticAlgorithm::start() {
  log.info("Starting genetic algorithm with population size: " +
           to_string(population.size()));
  stopping->handle_start();
  while (!stopping->should_stop()) {
    log.debug("Starting new generation");
    vector<Chromosome> parents = selection->select(population);
    log.debug("Selected " + to_string(parents.size()) +
              " parents for crossover");
    vector<Chromosome> offspring = crossover->recombine(parents);
    log.debug("Generated " + to_string(offspring.size()) +
              " offspring from crossover");
    vector<Chromosome> mutated = mutation->mutate(offspring);
    log.debug("Applied mutation, resulting in " + to_string(mutated.size()) +
              " mutated offspring");
    population = replacement->replace(population, offspring);
    log.debug("Replaced generation");
    generation++;
  }
  log.info("Genetic algorithm completed in " + to_string(generation) +
           " generations");
}
