#include <iostream>

#include "cache.hpp"
#include "distribution.hpp"

std::string orderingName(Ordering order);
std::string distributionName(Distribution dist);

int main() {
  const size_t K_MAX = 50;
  size_t sizes[] = { 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  Strategy strategies[] = { FIFO, FWF, LRU, LFU, RAND/*, RMA */ };
  uint16_t(*(distributions[]))() = {
    Distribution::uniform,
    Distribution::harmonic,
    Distribution::biharmonic,
    Distribution::geometric
  };

  for (Strategy strategy : strategies) {
    for (auto distribution : distributions) {
      for (size_t n : sizes) {
        size_t totalCost = 0;
        for (size_t k = 0; k < K_MAX; k++) {

          std::vector<size_t> counter(101, 0);
          for (size_t i = 0; i < n; i++) {
            uint16_t random = randomFromDist(distribution);
            if (random < 1 || random > 100) {
              std::cout << random << std::endl;
            }
            totalCost += access(list, random, ordering, &counter);
          }
        }
        std::cout << n << ";" << orderingName(ordering) << ";"
          << distributionName(distribution) << ";" << totalCost / K_MAX
          << std::endl;
      }
    }
  }

  return 0;
}

std::string orderingName(Ordering order) {
  switch (order) {
  case NONE:
    return "none";

  case MOVE_TO_FRONT:
    return "move-to-front";

  case TRANSPOSE:
    return "transpose";

  case COUNT:
    return "count";
  }
}

std::string distributionName(Distribution dist) {
  switch (dist) {
  case UNIFORM:
    return "uniform";

  case HARMONIC:
    return "harmonic";

  case BIHARMONIC:
    return "biharmonic";

  case GEOMETRIC:
    return "geometric";
  }
}
