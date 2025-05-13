#ifndef TSP_SOLUTION_ENCODER_H
#define TSP_SOLUTION_ENCODER_H
#include <vector>

#include "chromosome.h"

class TSPPathChromosome final : public Chromosome {
    std::vector<int> path;

public:
    TSPPathChromosome();
};

#endif //TSP_SOLUTION_ENCODER_H
