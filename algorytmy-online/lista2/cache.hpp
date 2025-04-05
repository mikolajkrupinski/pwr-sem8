#ifndef CACHE_HPP
#define CACHE_HPP

#include <cstddef>
#include <cstdint>
#include <vector>

enum Strategy {
  FIFO,  // First In First Out
  FWF,   // Flush When Full
  LRU,   // Least Recently Used
  LFU,   // Least Frequently Used
  RAND,  // Random
  RMA    // Randomized Markup Algorithm
};

class Cache {
 private:
  size_t k;
  size_t n;
  std::vector<uint16_t> inCache;
  size_t timeStamp;
  std::vector<size_t> timeStamps;
  std::vector<size_t> accessCount;
  std::vector<size_t> markedPages;

  bool find(uint16_t i);
  bool insert(uint16_t i);

 public:
  Cache(size_t k, size_t n) : k(k), n(n), timeStamp(0) {
    timeStamps.resize(n + 1, 0);
    accessCount.resize(n + 1, 0);
  }
  size_t access(uint16_t i, Strategy strategy);
};

#endif  // CACHE_HPP
