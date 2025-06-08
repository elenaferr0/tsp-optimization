#ifndef STOPPING_CRITERIA_H
#define STOPPING_CRITERIA_H
#include "utils/logger.h"

class StoppingCriterion {
protected:
    Logger log;
public:
    explicit StoppingCriterion(Logger::Level log_level);
    virtual ~StoppingCriterion() = default;
    virtual void handle_start() {}; // Method that will be called when the algorithm starts
    virtual bool should_stop() const = 0;
};

#endif //STOPPING_CRITERIA_H
