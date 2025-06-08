#include "genetic_algorithm/replacement/replacement.h"

Replacement::Replacement(const Logger::Level log_level) : log(log_level)
{
    log.set_label("Replacement");
}