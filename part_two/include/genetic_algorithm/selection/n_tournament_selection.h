#ifndef N_TOURNAMENT_SELECTION_H
#define N_TOURNAMENT_SELECTION_H
#include "selection_op.h"

class NTournamentSelection final : public SelectionOp {
public:
    explicit NTournamentSelection(Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> select(const HyperParams &params, const vector<Chromosome> &population) const override;
};

#endif // N_TOURNAMENT_SELECTION_H
