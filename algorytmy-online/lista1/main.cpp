#include <iostream>
#include <random>
#include <vector>

enum Ordering {
  NONE,
  MOVE_TO_FRONT,
  TRANSPOSE,
  COUNT,
};

enum Distribution {
  UNIFORM,
  HARMONIC,
  BIHARMONIC,
  GEOMETRIC,
};

size_t access(std::vector<uint16_t> &list, uint16_t i, Ordering order,
              std::vector<size_t> *counter);
double harmonic(size_t n);
double biharmonic(size_t n);
uint16_t randomFromDist(Distribution dist);
std::string orderingName(Ordering order);
std::string distributionName(Distribution dist);

int main() {
  const size_t K_MAX = 50;
  size_t sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
  Ordering orderings[] = {NONE, MOVE_TO_FRONT, TRANSPOSE, COUNT};
  Distribution distributions[] = {UNIFORM, HARMONIC, BIHARMONIC, GEOMETRIC};

  for (Ordering ordering : orderings) {
    for (Distribution distribution : distributions) {
      for (size_t n : sizes) {
        size_t totalCost = 0;
        for (size_t k = 0; k < K_MAX; k++) {
          std::vector<uint16_t> list;
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

size_t access(std::vector<uint16_t> &list, uint16_t i, Ordering order,
              std::vector<size_t> *counter) {
  size_t cost = 1;

  auto curr = list.begin();
  while (curr != list.end() && *curr != i) {
    cost++;
    curr++;
  }

  if (curr == list.end()) {
    list.push_back(i);
    curr = list.end() - 1;
  }

  switch (order) {
    case NONE:
      break;

    case MOVE_TO_FRONT:
      list.erase(curr);
      list.insert(list.begin(), i);
      break;

    case TRANSPOSE:
      if (list.size() > 1 && curr != list.begin()) {
        std::swap(*(curr - 1), *curr);
      }
      break;

    case COUNT:
      if (counter != nullptr && counter->size() == 101) {
        (*counter)[i]++;
        while (curr != list.begin() &&
               (*counter)[*(curr - 1)] < (*counter)[*curr]) {
          std::swap(*(curr - 1), *curr);
          curr--;
        }
      }
      break;
  }

  return cost;
}

double harmonic(size_t n) {
  double res = 0.0;
  for (size_t i = n; i >= 1; i--) {
    res += 1.0 / (double)(i);
  }
  return res;
}

double biharmonic(size_t n) {
  double res = 0.0;
  for (size_t i = n; i >= 1; i--) {
    res += 1.0 / (double)(i * i);
  }
  return res;
}

uint16_t randomFromDist(Distribution dist) {
  static std::random_device dev;
  static std::mt19937 rng(dev());

  static std::uniform_int_distribution<uint16_t> uniform(1, 100);
  static std::uniform_real_distribution<double> uniformDouble(0.0, 1.0);
  static std::bernoulli_distribution bernoulli(0.5);

  static double harmonic100 = harmonic(100);
  static double biharmonic100 = biharmonic(100);

  double cumulative, u;
  switch (dist) {
    case UNIFORM:
      return uniform(rng);

    case HARMONIC:
      cumulative = 0.0;
      u = uniformDouble(rng);
      for (int k = 1; k <= 100; k++) {
        cumulative += (1.0 / (k * harmonic100));
        if (u <= cumulative) {
          return k;
        }
      }
      return 100;

    case BIHARMONIC:
      cumulative = 0.0;
      u = uniformDouble(rng);
      for (int k = 1; k <= 100; ++k) {
        cumulative += (1.0 / (k * k * biharmonic100));
        if (u <= cumulative) {
          return k;
        }
      }
      return 100;

    case GEOMETRIC:
      for (uint16_t i = 1; i <= 100; i++) {
        if (bernoulli(rng)) {
          return i;
        }
      }
      return 100;
  }

  return 1;
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
