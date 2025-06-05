#include "genetic_algorithm/mutation/displacement_mutation.h"

DisplacementMutation::DisplacementMutation(double rate) : MutationOp(rate) {

}

vector<Chromosome> DisplacementMutation::mutate(const vector<Chromosome> &population) const {
    return population; // TODO
}
