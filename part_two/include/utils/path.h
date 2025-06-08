#ifndef PATH_UTILS_H
#define PATH_UTILS_H

#include "utils/typedefs.h"
#include "utils/maths.h"

// Generates two random cut points for a path of length n
inline priority_queue_asc<int> random_cut_points(int parents_len) {
  priority_queue_asc<int> cuts;
  int n_cuts = 2;

  while (cuts.size() < n_cuts) {
    int cut = unif(0, parents_len - 1);
    if (cuts.empty() || cuts.top() - cut > 1) { // ensure cuts are not adjacent
      cuts.push(cut);
    }
  }

  return cuts;
}

#endif //PATH_UTILS_H