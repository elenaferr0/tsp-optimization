#import "genetic_algorithm/chromosome/chromosome.h"

bool Chromosome::operator<(const Chromosome &other) const {
    return this->evaluate_fitness() < other.evaluate_fitness();
}