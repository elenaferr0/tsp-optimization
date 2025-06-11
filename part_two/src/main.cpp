#include <iostream>
#include <memory>

#include "tsp/graph.h"
#include <utils/path.h>

#include "genetic_algorithm/genetic_algorithm.h"
#include "genetic_algorithm/mutation/displacement_mutation.h"
#include "genetic_algorithm/initialization/convex_hull_initialization.h"

class SelectionOp;
using namespace std;

Chromosome create_chromosome(const int size, const int min_id = 0) {
  // Create a simple graph with 'size' nodes
  vector<Node> nodes;
  for (int i = 0; i < size; ++i) {
    auto random_x = static_cast<double>(rand() % 100);
    auto random_y = static_cast<double>(rand() % 100);
    nodes.emplace_back(i, random_x, random_y);
  }
  return Chromosome(Graph(nodes));
}

int main(const int argc, char *argv[]) {
  try {
    auto level = Logger::Level::DEBUG;
    ConvexHullInitialization ch(level, Graph::from_file("random_10"), 10);
    auto population = ch.generate_population();
    for (const auto &chromosome : population) {
      std::cout << chromosome << std::endl;
    }
  } catch (std::exception &e) {
    std::cout << "Exception: " << e.what() << std::endl;
  }
  return 0;
}
