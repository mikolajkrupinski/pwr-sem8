#include <iostream>
#include <random>
#include <vector>

double get_approx(size_t n);

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ex17.out <no of stations>" << std::endl;
    return 1;
  }

  size_t n = atol(argv[1]);

  std::cout << "n;approx" << std::endl;
  for (size_t k = 1; k <= n; k++) {
    std::cout << k << ";" << get_approx(k) << std::endl;
  }

  return 0;
}

double get_approx(size_t n) {
  static std::random_device dev;
  static std::mt19937 gen(dev());
  static std::uniform_real_distribution<double> rand(0, 1);

  if (n <= 400) {
    return n;
  }

  std::vector<double> dzetas(n);
  for (size_t i = 0; i < n; i++) {
    dzetas[i] = rand(gen);
  }
  std::sort(dzetas.begin(), dzetas.end());

  return 399.0 / dzetas[399];
}