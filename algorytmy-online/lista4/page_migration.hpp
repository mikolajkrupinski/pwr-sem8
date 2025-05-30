#ifndef PAGE_MIGRATION_HPP
#define PAGE_MIGRATION_HPP

#include <vector>

#include "graph.hpp"

class PageMigration {
 private:
  Graph graph;
  size_t resource_node;
  size_t d;
  std::vector<size_t> prompted_nodes_in_phase;

  size_t find_min_from_phase() const;

 public:
  PageMigration(Graph &graph, size_t d)
      : graph(graph), resource_node(0), d(d) {};

  size_t move_to_min(size_t prompt_node);

  size_t coin_flip(size_t prompt_node);
};

#endif  // PAGE_MIGRATION_HPP
