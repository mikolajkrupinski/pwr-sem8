#include <fstream>
#include <iostream>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "Usage: ex16.out <no of balls> <no of buckets> <out file name>"
              << std::endl;
    return 1;
  }

  size_t m = atol(argv[1]);  // no of balls
  size_t n = atol(argv[2]);  // no of buckets
  std::ofstream out_file(argv[3]);

  std::cout << std::endl;
  std::cout << "No of balls   = " << m << std::endl;
  std::cout << "No of buckets = " << n << std::endl;

  out_file << "method;bucket;load" << std::endl;

  std::random_device dev;
  std::mt19937 gen(dev());
  std::uniform_int_distribution<size_t> rand(0, n - 1);

  // Uniform Placement
  {
    std::vector<size_t> balls_in_bucket(n, 0);
    size_t no_of_empty_buckets = n;
    size_t max_load = 0;
    for (size_t i = 0; i < m; i++) {
      size_t j = rand(gen);
      if (balls_in_bucket[j] == 0) {
        no_of_empty_buckets--;
      }
      balls_in_bucket[j]++;
      if (max_load < balls_in_bucket[j]) {
        max_load = balls_in_bucket[j];
      }
    }

    std::cout << std::endl;
    std::cout << "UNIFORM PLACEMENT" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "No of empty buckets = " << no_of_empty_buckets << std::endl;
    std::cout << "Max load            = " << max_load << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < n; i++) {
      out_file << "uniform;" << i + 1 << ";" << balls_in_bucket[i] << std::endl;
    }
  }

  // Power of two chioces
  {
    std::vector<size_t> balls_in_bucket(n, 0);
    size_t no_of_empty_buckets = n;
    size_t max_load = 0;
    for (size_t i = 0; i < m; i++) {
      size_t k = rand(gen), l = rand(gen);
      size_t j = balls_in_bucket[k] <= balls_in_bucket[l] ? k : l;
      if (balls_in_bucket[j] == 0) {
        no_of_empty_buckets--;
      }
      balls_in_bucket[j]++;
      if (max_load < balls_in_bucket[j]) {
        max_load = balls_in_bucket[j];
      }
    }

    std::cout << std::endl;
    std::cout << "POWER OF TWO CHOICES" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "No of empty buckets = " << no_of_empty_buckets << std::endl;
    std::cout << "Max load            = " << max_load << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << std::endl;

    for (size_t i = 0; i < n; i++) {
      out_file << "power;" << i + 1 << ";" << balls_in_bucket[i] << std::endl;
    }
  }

  out_file.close();

  return 0;
}
