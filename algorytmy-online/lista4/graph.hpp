#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>

class Graph {
 private:
  size_t no_of_nodes;
  std::vector<std::vector<size_t>> adjacency_list;
  std::vector<std::vector<size_t>> distances;

  size_t get_distance(size_t source, size_t target) const;

  void calculate_distances();

 public:
  void make_3d_torus(size_t no_of_nodes_per_dimension);

  void make_hypercube(size_t bit_sequence_length);

  size_t get_no_of_nodes() const { return no_of_nodes; }

  size_t get_min_distance(size_t source, size_t target) const {
    return distances[source][target];
  }
};

#endif  // GRAPH_HPP
