#include "genetic_algorithm/stopping/max_generations_criterion.h"

MaxGenerationsCriterion::MaxGenerationsCriterion(const Logger::Level log_level, long max_generations)
    : StoppingCriterion(log_level), max_generations(max_generations), current_generation(0) {
    if (max_generations <= 0) {
        throw std::invalid_argument("Max generations must be greater than 0");
    }

    log.set_label("MaxGenerationsCriterion");
}

void MaxGenerationsCriterion::handle_start() {
    current_generation = 0;
    log.info("Starting max generations criterion with limit: " + std::to_string(max_generations));
}

bool MaxGenerationsCriterion::should_stop() {
    log.debug("Current generation: " + std::to_string(current_generation) +
              ", Max generations: " + std::to_string(max_generations));
    current_generation++;
    return current_generation >= max_generations;
}