#include <iostream>
#include <random>

#include "bins.hpp"
#include "distribution.hpp"

std::string strategyToString(Strategy strategy);
std::string distrubutionTypeToString(DistributionType distType);

int main() {
  const size_t NO_OF_REPETITIONS = 10000;
  const size_t LENGTH_OF_SEQUENCE = 100;
  Strategy strategies[] = {NEXT_FIT, RANDOM_FIT, FIRST_FIT, BEST_FIT,
                           WORST_FIT};
  DistributionType distTypes[] = {UNIFORM, HARMONIC, BIHARMONIC, GEOMETRIC};
  Distribution dist;
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<double> ud(0.0, 1.0);

  std::cout << "strategy;distribution;avg-bins;avg-opt-bins;competitive-ratio"
            << std::endl;
  for (Strategy strategy : strategies) {
    for (DistributionType distType : distTypes) {
      size_t sum_of_no_of_bins = 0;
      size_t opt_sum_of_no_of_bins = 0;
      for (size_t i = 0; i < NO_OF_REPETITIONS; i++) {
        Bins bins(strategy);
        size_t no_of_elements = 0;
        double sum_of_elements = 0.0;
        while (no_of_elements < LENGTH_OF_SEQUENCE) {
          double x = ud(rng);
          size_t k = std::min((size_t)dist.randFromDist(distType, 10),
                              LENGTH_OF_SEQUENCE - no_of_elements);
          for (size_t j = 0; j < k; j++) {
            bins.add_element(x);
            sum_of_elements += x;
          }
          no_of_elements += k;
        }
        sum_of_no_of_bins += bins.get_no_of_bins();
        opt_sum_of_no_of_bins += (size_t)ceil(sum_of_elements);
      }
      double avg_no_of_bins = (double)sum_of_no_of_bins / NO_OF_REPETITIONS;
      double avg_opt_no_of_bins =
          (double)opt_sum_of_no_of_bins / NO_OF_REPETITIONS;
      double concurrency = avg_no_of_bins / avg_opt_no_of_bins;
      std::cout << strategyToString(strategy) << ";"
                << distrubutionTypeToString(distType) << ";" << avg_no_of_bins
                << ";" << avg_opt_no_of_bins << ";" << concurrency << std::endl;
    }
  }

  return 0;
}

std::string strategyToString(Strategy strategy) {
  switch (strategy) {
    case NEXT_FIT:
      return "next-fit";
    case RANDOM_FIT:
      return "random-fit";
    case FIRST_FIT:
      return "first-fit";
    case BEST_FIT:
      return "best-fit";
    case WORST_FIT:
      return "worst-fit";
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
