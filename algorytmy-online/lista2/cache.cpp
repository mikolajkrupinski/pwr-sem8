#include "cache.hpp"

#include <algorithm>
#include <iostream>
#include <random>

bool Cache::find(uint16_t i) {
  return std::find(inCache.begin(), inCache.end(), i) != inCache.end();
}

bool Cache::insert(uint16_t i) {
  if (inCache.size() == k) {
    return false;
  }

  inCache.push_back(i);
  return true;
}

size_t Cache::access(uint16_t i, Strategy strategy) {
  if (i > n) {
    std::cerr << "Error: Accessing out of bounds index " << i << std::endl;
  }

  timeStamp++;
  timeStamps[i] = timeStamp;
  accessCount[i]++;

  if (std::find(markedPages.begin(), markedPages.end(), i) ==
    markedPages.end()) {
    markedPages.push_back(i);
  }

  if (find(i)) {
    return 0;
  }
  if (insert(i)) {
    return 1;
  }

  static std::random_device dev;
  static std::mt19937 rng(dev());

  switch (strategy) {
  case FIFO:
    inCache.erase(inCache.begin());
    inCache.push_back(i);
    break;

  case FWF:
    inCache.clear();
    inCache.push_back(i);
    break;

  case LRU: {
    size_t minTimeStamp = timeStamps[inCache[0]];
    size_t minIndex = 0;
    for (size_t j = 1; j < k; j++) {
      if (timeStamps[inCache[j]] < minTimeStamp) {
        minTimeStamp = timeStamps[inCache[j]];
        minIndex = j;
      }
    }
    inCache.erase(inCache.begin() + minIndex);
    inCache.push_back(i);
    break;
  }

  case LFU: {
    size_t minAccessCount = accessCount[inCache[0]];
    size_t minIndex = 0;
    for (size_t j = 1; j < k; j++) {
      if (accessCount[inCache[j]] < minAccessCount) {
        minAccessCount = accessCount[inCache[j]];
        minIndex = j;
      }
    }
    inCache.erase(inCache.begin() + minIndex);
    inCache.push_back(i);
    break;
  }

  case RAND: {
    std::uniform_int_distribution<size_t> u(0, k - 1);
    size_t randIndex = u(rng);
    inCache.erase(inCache.begin() + randIndex);
    inCache.push_back(i);
    break;
  }

  case RMA: {
    if (markedPages.size() >= k) {
      markedPages.clear();
      markedPages.push_back(i);
    }
    std::vector<uint16_t> unmarkedPagesIndices;
    for (size_t j = 0; j < k; j++) {
      if (std::find(markedPages.begin(), markedPages.end(), inCache[j]) ==
        markedPages.end()) {
        unmarkedPagesIndices.push_back(j);
      }
    }
    std::uniform_int_distribution<size_t> uniformUnmarked(
      0, unmarkedPagesIndices.size() - 1);
    inCache.erase(inCache.begin() +
      unmarkedPagesIndices[uniformUnmarked(rng)]);
    inCache.push_back(i);
    break;
  }
  }

  return 1;
}
