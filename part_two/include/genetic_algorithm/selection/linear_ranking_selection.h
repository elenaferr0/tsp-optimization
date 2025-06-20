#ifndef LINEAR_RANKING_SELECTION_H
#define LINEAR_RANKING_SELECTION_H
#include "genetic_algorithm/chromosome/chromosome.h"
#include "selection_op.h"

class LinearRankingSelection final : public SelectionOp {
public:
    explicit LinearRankingSelection(const Logger::Level log_level);

    [[nodiscard]] string name() const override;

    vector<Chromosome> select(const HyperParams &params, const vector<Chromosome> &population) const override;
};

#endif // LINEAR_RANKING_SELECTION_H
