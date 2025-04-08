#include <iostream>

#include "cache.hpp"
#include "distribution.hpp"

std::string strategyToString(Strategy strategy);
std::string distrubutionTypeToString(DistributionType distType);

int main() {
  const size_t NO_OF_REPETITIONS = 10000;
  size_t sizes[] = { 20, 30, 40, 50, 60, 70, 80, 90, 100 };
  size_t fraction_min = 10, fraction_max = 5;
  Strategy strategies[] = { FIFO, FWF, LRU, LFU, RAND, RMA };
  DistributionType distTypes[] = { UNIFORM, HARMONIC, BIHARMONIC, GEOMETRIC };
  Distribution dist;

  std::cout << "n;k;strategy;distribution;cost" << std::endl;
  for (Strategy strategy : strategies) {
    for (DistributionType distType : distTypes) {
      for (size_t n : sizes) {
        for (size_t k = n / fraction_min; k <= n / fraction_max; k++) {
          size_t totalAccessCost = 0;
          Cache cache(k, n);
          for (size_t i = 0; i < n * NO_OF_REPETITIONS; i++) {
            totalAccessCost +=
              cache.access(dist.randFromDist(distType, n), strategy);
          }
          std::cout << n << ";" << k << ";" << strategyToString(strategy) << ";"
            << distrubutionTypeToString(distType) << ";"
            << totalAccessCost / (double)(n * NO_OF_REPETITIONS)
            << std::endl;
        }
      }
    }
  }

  return 0;
}

std::string strategyToString(Strategy strategy) {
  switch (strategy) {
  case FIFO:
    return "fifo";
  case FWF:
    return "fwf";
  case LRU:
    return "lru";
  case LFU:
    return "lfu";
  case RAND:
    return "rand";
  case RMA:
    return "rma";
  }
  return "none";
}

std::string distrubutionTypeToString(DistributionType distType) {
  switch (distType) {
  case UNIFORM:
    return "uniform";
  case HARMONIC:
    return "harmonic";
  case BIHARMONIC:
    return "biharmonic";
  case GEOMETRIC:
    return "geometric";
  }
  return "none";
}
