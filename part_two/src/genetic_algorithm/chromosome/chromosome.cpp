#include "genetic_algorithm/chromosome/chromosome.h"

#include <memory>
#include <sstream>
#include <stdexcept>

Chromosome::Chromosome(const Graph &graph) : graph(graph) {
  if (graph.path.size() == 0) {
    throw std::invalid_argument("Path cannot be empty");
  }
}

size_t Chromosome::get_n_genes() const { return graph.path.size(); }

Node Chromosome::get_node(int i) const {
  if (i < 0 || i >= graph.path.size()) {
    throw std::out_of_range("Index out of range");
  }
  return graph[i];
}

string Chromosome::to_str() const {
  stringstream ss;
  ss << "Path: ";
  for (size_t i = 0; i < graph.path.size(); ++i) {
    if (i > 0)
      ss << " → ";
    ss << graph[static_cast<int>(i)].id;
    ss << "(" << graph[static_cast<int>(i)].x() << ", "
       << graph[static_cast<int>(i)].position.second << ")";
  }
  ss << " | Fitness: " << evaluate_fitness();
  return ss.str();
}

bool Chromosome::operator<(const Chromosome &other) const {
  return this->evaluate_fitness() < other.evaluate_fitness();
}

vector<Node> Chromosome::get_subpath(int start, int end) const {
  if (start < 0 || end >= graph.path.size() || start > end) {
    throw out_of_range("Invalid subpath indices");
  }
  return vector<Node>(graph.path.begin() + start, graph.path.begin() + end + 1);
}

void Chromosome::set_subpath(int start, int end, vector<Node> new_path) {
  if (start < 0 || end >= graph.path.size() || start > end) {
    throw out_of_range("Invalid subpath indices");
  }
  if (new_path.size() != (end - start + 1)) {
    throw invalid_argument("New path size does not match the specified range");
  }
  copy(new_path.begin(), new_path.end(), graph.path.begin() + start);
}

double Chromosome::evaluate_fitness() const {
  double fitness = 0.0;
  for (size_t i = 0; i < graph.path.size() - 1; ++i) {
    fitness += graph.get_cost(graph[i].id, graph[i + 1].id);
  }
  fitness +=
      graph.get_cost(graph.path.back().id,
                     graph.path.front().id); // Return to the starting point
  return fitness;
}

ostream &operator<<(ostream &os, const Chromosome &chromosome) {
  os << "Chromosome with path: ";
  for (const auto &node : chromosome.graph.path) {
    os << node.id << "(" << node.x() << ", " << node.position.second << ") ";
  }
  os << "with fitness: " << chromosome.evaluate_fitness();
  return os;
}