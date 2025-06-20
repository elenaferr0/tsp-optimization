#include "genetic_algorithm/stopping/max_non_improving_generations_criterion.h"

#include "utils/maths.h"

MaxNonImprovingGenerationsCriterion::MaxNonImprovingGenerationsCriterion(const Logger::Level log_level)
    : StoppingCriterion(Logger(log_level, "MaxNonImprovingGenerationsCriterion")),
      last_best_fitness(numeric_limits<double>::max()),
      non_improving_count(0) {
}

void MaxNonImprovingGenerationsCriterion::handle_start(const HyperParams &params) {
    log.debug("Starting max generations criterion with limit: " +
              std::to_string(params.max_non_improving_generations));
}

bool MaxNonImprovingGenerationsCriterion::should_stop(const HyperParams &params, const double best_fitness) {
    log.trace("Checking stop condition: best fitness = " + std::to_string(best_fitness) +
              ", last best fitness = " + std::to_string(last_best_fitness) +
              ", non-improving count = " + std::to_string(non_improving_count));

    if (non_improving_count >= params.max_non_improving_generations) {
        log.debug(
            "Stopping due to non-improvement for " + std::to_string(params.max_non_improving_generations) +
            " generations.");
        return true;
    }

    if (eq(last_best_fitness, best_fitness)) {
        non_improving_count++;
    } else {
        last_best_fitness = best_fitness;
        non_improving_count = 0;
    }
    log.trace("Non-improving count: " + std::to_string(non_improving_count));
    return false;
}
