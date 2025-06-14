#ifndef LINEAR_RANKING_SELECTION_H
#define LINEAR_RANKING_SELECTION_H
#include "genetic_algorithm/chromosome/chromosome.h"
#include "selection_op.h"

class LinearRankingSelection final : public SelectionOp {
public:
  explicit LinearRankingSelection(Logger::Level log_level, const HyperParams &params);

  vector<Chromosome> select(const vector<Chromosome> &population) override;
};

#endif // LINEAR_RANKING_SELECTION_H
