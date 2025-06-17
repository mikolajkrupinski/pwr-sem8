#include <iostream>
#include <random>
#include <vector>

class Reservoir {
private:
  size_t n;
  size_t l;
  double p;

public:
  Reservoir() : n(0) {}

  void on_get(double x) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    n++;
    if (dist(gen) < 1.0 / n) {
      std::cout << n << " ";
      l = n;
      p = x;
    }
  }
};

class ReservoirFastForward {
private:
  size_t n;
  std::vector<size_t> next;
  size_t next_idx;
  size_t l;
  double p;

public:
  ReservoirFastForward() : n(0), next_idx(0) {}

  void on_init(size_t max_size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    next.clear();
    size_t n = 0;
    next.push_back(1);
    while (n <= max_size) {
      n++;
      if (n == next.back()) {
        std::cout << n << " ";
        double u = dist(gen);
        size_t _next = n + (size_t)ceil((u * n) / (1 - u));
        next.push_back(_next);
      }
    }
  }

  void on_get(double x) {
    n++;
    if (n == next[next_idx]) {
      l = n;
      p = x;
      next_idx++;
    }
  }
};

int main() {
  const size_t STREAM_LENGTH = 50;

  Reservoir r;
  for (size_t i = 0; i < STREAM_LENGTH; i++) {
    r.on_get(0.0);
  }
  std::cout << std::endl;

  ReservoirFastForward rff;
  rff.on_init(STREAM_LENGTH);
  std::cout << std::endl;

  return 0;
}