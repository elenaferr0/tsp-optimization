#include "genetic_algorithm/replacement/steady_state_replacement.h"

SteadyStateReplacement::SteadyStateReplacement() : Replacement() {
    // Constructor implementation (if needed)
}

vector<Chromosome> SteadyStateReplacement::replace(const vector<Chromosome> &parents,
    const vector<Chromosome> &offsprings) const {
    return parents; // TODO
}
