#include "page_migration.hpp"

#include <random>

size_t PageMigration::move_to_min(size_t prompt_node) {
  size_t cost = graph.get_min_distance(resource_node, prompt_node);
  prompted_nodes_in_phase.push_back(prompt_node);

  if (prompted_nodes_in_phase.size() >= d) {
    size_t min_node = find_min_from_phase();
    prompted_nodes_in_phase.clear();
    cost += d * graph.get_min_distance(resource_node, min_node);
    resource_node = min_node;
  }

  return cost;
}

size_t PageMigration::coin_flip(size_t prompt_node) {
  size_t cost = graph.get_min_distance(resource_node, prompt_node);

  static std::random_device dev;
  static std::mt19937 rng(dev());
  static std::uniform_real_distribution<double> uniform(0.0, 1.0);

  if (uniform(rng) < 1.0 / (2.0 * d)) {
    cost *= d + 1;
    resource_node = prompt_node;
  }

  return cost;
}

size_t PageMigration::find_min_from_phase() const {
  size_t min_total_dist = -1;
  size_t min_node = -1;
  for (size_t u = 0; u < graph.get_no_of_nodes(); u++) {
    size_t total_dist = 0;
    for (size_t v : prompted_nodes_in_phase) {
      total_dist += graph.get_min_distance(u, v);
    }
    if (total_dist < min_total_dist) {
      min_total_dist = total_dist;
      min_node = u;
    }
  }
  return min_node;
}