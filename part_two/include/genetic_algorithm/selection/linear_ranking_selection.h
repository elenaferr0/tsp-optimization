#ifndef LINEAR_RANKING_SELECTION_H
#define LINEAR_RANKING_SELECTION_H
#include "selection_op.h"

class LinearRankingSelection final : public SelectionOp {
  public:
    explicit LinearRankingSelection(int n_parents);
    vector<Chromosome> select(const vector<Chromosome>& population) override;
};

#endif //LINEAR_RANKING_SELECTION_H
