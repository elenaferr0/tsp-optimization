#ifndef STOPPING_CRITERIA_H
#define STOPPING_CRITERIA_H
#include "utils/logger.h"

class StoppingCriterion {
protected:
    Logger log;
public:
    explicit StoppingCriterion(Logger::Level log_level);
    virtual ~StoppingCriterion() = default;
    virtual void handle_start() = 0;
    virtual bool should_stop() = 0;
};

#endif //STOPPING_CRITERIA_H
