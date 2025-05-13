#ifndef SOLUTION_H
#define SOLUTION_H

class Chromosome {
public:
    virtual ~Chromosome() = default;
    virtual double evaluate_fitness() const = 0;
    bool operator<(const Chromosome &other) const;
};

#endif //SOLUTION_H
