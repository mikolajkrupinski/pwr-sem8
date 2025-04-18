#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <random>

enum DistributionType { UNIFORM, HARMONIC, BIHARMONIC, GEOMETRIC };

class Distribution {
private:
  std::random_device dev;
  std::mt19937 rng;

  double _harmonic(uint16_t n);
  double _biharmonic(uint16_t n);

public:
  Distribution() { rng.seed(dev()); }

  uint16_t uniform(uint16_t max);
  uint16_t harmonic(uint16_t n);
  uint16_t biharmonic(uint16_t n);
  uint16_t geometric(uint16_t n);
  uint16_t randFromDist(DistributionType distType, uint16_t n);
};

#endif  // DISTRIBUTION_HPP
