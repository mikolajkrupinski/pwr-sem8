#include "graph.hpp"

#include <queue>

void Graph::make_3d_torus(size_t no_of_nodes_per_dimension) {
  size_t k = no_of_nodes_per_dimension;
  no_of_nodes = k * k * k;
  adjacency_list.resize(no_of_nodes);

  size_t pow[] = {1, k, k * k, k * k * k};
  for (size_t u = 0; u < no_of_nodes; u++) {
    for (size_t i = 0; i < 3; i++) {
      adjacency_list[u].push_back((u + pow[i]) % pow[i + 1] +
                                  (pow[i + 1] * (u / pow[i + 1])));
      adjacency_list[u].push_back((u - pow[i]) % pow[i + 1] +
                                  (pow[i + 1] * (u / pow[i + 1])));
    }
  }

  calculate_distances();
}

void Graph::make_hypercube(size_t bit_sequence_length) {
  no_of_nodes = 1 << bit_sequence_length;
  adjacency_list.resize(no_of_nodes);

  for (size_t i = 0; i < no_of_nodes; i++) {
    for (size_t j = 0; j < bit_sequence_length; j++) {
      size_t neighbor = i ^ (1 << j);
      adjacency_list[i].push_back(neighbor);
    }
  }

  calculate_distances();
}

size_t Graph::get_distance(size_t source, size_t target) const {
  std::vector<bool> visited(no_of_nodes, false);
  std::vector<size_t> dist(no_of_nodes, 0);
  std::queue<size_t> queue;

  visited[source] = true;
  queue.push(source);
  while (!queue.empty()) {
    size_t current = queue.front();
    queue.pop();
    if (current == target) {
      break;
    }
    for (size_t neighbor : adjacency_list[current]) {
      if (!visited[neighbor]) {
        visited[neighbor] = true;
        dist[neighbor] = dist[current] + 1;
        queue.push(neighbor);
      }
    }
  }

  return dist[target];
}

void Graph::calculate_distances() {
  distances.resize(no_of_nodes, std::vector<size_t>(no_of_nodes, 0));
  for (size_t u = 0; u < no_of_nodes; u++) {
    for (size_t v = u; v < no_of_nodes; v++) {
      distances[u][v] = get_distance(u, v);
      distances[v][u] = distances[u][v];
    }
  }
}
