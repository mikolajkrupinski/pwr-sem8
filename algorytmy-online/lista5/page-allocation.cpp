#include "page-allocation.hpp"

PageAllocation::PageAllocation(uint32_t graph_size, uint32_t D) : graph_size(graph_size), D(D) {
    this->counter.resize(graph_size, 0);

    this->waiting = true;
    this->resource_node = 0;
}


uint32_t PageAllocation::read(uint32_t node) {
    if (this->resource_node == node) {
        return 0;
    }

    if (this->counter[node] < this->D && this->waiting) {
        this->counter[node]++;
        if (this->counter[node] == this->D) {
            this->resource_node = node;
            this->waiting = false;
            return 1 + this->D;
        }
    }

    return 1;
}

uint32_t PageAllocation::write(uint32_t node) {
    if (this->resource_node == node) {
        return 0;
    }

    if (!this->waiting) {
        this->counter[this->resource_node]--;
        if (this->counter[this->resource_node] == 0) {
            this->waiting = true;
        }
    }

    return 1;
}
