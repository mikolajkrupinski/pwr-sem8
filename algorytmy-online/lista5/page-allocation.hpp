#ifndef PAGE_ALLOCATION_HPP
#define PAGE_ALLOCATION_HPP

#include <cstddef>
#include <vector>

class PageAllocation {
 private:
  size_t graph_size;
  std::vector<bool> resource;

 public:
  PageAllocation();
  uint32_t read(uint32_t node);
  uint32_t write();
};

#endif  // PAGE_ALLOCATION_HPP
