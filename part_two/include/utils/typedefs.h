#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#include <memory>
#include <vector>

#include "genetic_algorithm/chromosome/chromosome.h"

using namespace std;

typedef vector<vector<bool>> BitMat;
typedef vector<vector<double>> DblMat;
typedef vector<vector<int>> IntMat;

typedef vector<int> IntVec;

typedef shared_ptr<Chromosome> ChromosomePtr;

#endif
