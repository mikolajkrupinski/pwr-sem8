#ifndef PAGE_ALLOCATION_HPP
#define PAGE_ALLOCATION_HPP

#include <vector>
#include <cstdint>

// Page allocation on complete graph given size (number of nodes)
class PageAllocation {
private:
  const uint32_t D;
  const uint32_t graph_size;

  bool waiting;
  uint32_t resource_node;
  std::vector<uint32_t> counter;

public:
  PageAllocation(uint32_t graph_size, uint32_t D);
  uint32_t read(uint32_t node);
  uint32_t write(uint32_t node);
};

#endif  // PAGE_ALLOCATION_HPP
