#include <iostream>

#include "distribution.hpp"
#include "graph.hpp"
#include "page_migration.hpp"

std::string to_string(DistributionType dist_type);

int main() {
  const size_t D[] = {16, 32, 64, 128, 256};
  const size_t N = 65536;
  const size_t K = 100;
  const DistributionType dist_types[] = {DistributionType::UNIFORM,
                                         DistributionType::HARMONIC,
                                         DistributionType::BIHARMONIC};

  Distribution dist;
  Graph hypercube, torus;
  hypercube.make_hypercube(6);
  torus.make_3d_torus(4);

  std::cout << "algorithm;graph;d;dist;cost" << std::endl;
  for (size_t d : D) {
    for (DistributionType dist_type : dist_types) {
      // Hypercube, Move to min
      {
        size_t total_cost = 0;
        for (size_t k = 0; k < K; k++) {
          PageMigration pm(hypercube, d);
          for (size_t i = 0; i < N; i++) {
            size_t u = dist.randFromDist(dist_type, 64) - 1;
            total_cost += pm.move_to_min(u);
          }
        }
        std::cout << "move-to-min;hypercube;" << d << ";"
                  << to_string(dist_type) << ";" << total_cost / K << std::endl;
      }
      // Hypercube, Coin flip
      {
        size_t total_cost = 0;
        for (size_t k = 0; k < K; k++) {
          PageMigration pm(hypercube, d);
          for (size_t i = 0; i < N; i++) {
            size_t u = dist.randFromDist(dist_type, 64) - 1;
            total_cost += pm.coin_flip(u);
          }
        }
        std::cout << "coin-flip;hypercube;" << d << ";" << to_string(dist_type)
                  << ";" << total_cost / K << std::endl;
      }
      // Torus, Move to min
      {
        size_t total_cost = 0;
        for (size_t k = 0; k < K; k++) {
          PageMigration pm(torus, d);
          for (size_t i = 0; i < N; i++) {
            size_t u = dist.randFromDist(dist_type, 64) - 1;
            total_cost += pm.move_to_min(u);
          }
        }
        std::cout << "move-to-min;torus;" << d << ";" << to_string(dist_type)
                  << ";" << total_cost / K << std::endl;
      }
      // Torus, Coin flip
      {
        size_t total_cost = 0;
        for (size_t k = 0; k < K; k++) {
          PageMigration pm(torus, d);
          for (size_t i = 0; i < N; i++) {
            size_t u = dist.randFromDist(dist_type, 64) - 1;
            total_cost += pm.coin_flip(u);
          }
        }
        std::cout << "coin-flip;torus;" << d << ";" << to_string(dist_type)
                  << ";" << total_cost / K << std::endl;
      }
    }
  }

  return 0;
}

std::string to_string(DistributionType dist_type) {
  switch (dist_type) {
    case UNIFORM:
      return "uniform";
    case HARMONIC:
      return "harmonic";
    case BIHARMONIC:
      return "biharmonic";
    default:
      return "unknown";
  }
}
