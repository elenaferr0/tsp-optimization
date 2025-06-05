#ifndef N_TOURNAMENT_SELECTION_H
#define N_TOURNAMENT_SELECTION_H
#include "selection_op.h"

class NTournamentSelection final : public SelectionOp {
    int tournament_size;

public:
    explicit NTournamentSelection(int tournament_size);

    vector<Chromosome> select(const vector<Chromosome> &population) override;
};

#endif //N_TOURNAMENT_SELECTION_H
