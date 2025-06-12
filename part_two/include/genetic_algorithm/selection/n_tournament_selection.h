#ifndef N_TOURNAMENT_SELECTION_H
#define N_TOURNAMENT_SELECTION_H
#include "selection_op.h"

class NTournamentSelection final : public SelectionOp {
  int tournament_size;

public:
  explicit NTournamentSelection(Logger::Level log_level, int tournament_size, int n_parents);

  vector<Chromosome> select(const vector<Chromosome> &population) override;
};

#endif // N_TOURNAMENT_SELECTION_H
