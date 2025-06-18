#include "page-allocation.hpp"

PageAllocation::PageAllocation(uint32_t graph_size, uint32_t D) : graph_size(graph_size), D(D) {
    this->counter.resize(graph_size, 0);
    this->resource.resize(graph_size, false);

    this->waiting = true;
    this->resource[0] = true;
    this->nodes_with_resource = 1;
}


uint32_t PageAllocation::read(uint32_t node) {
    if (this->resource[node]) {
        return 0;
    }

    if (this->counter[node] < this->D) {
        this->counter[node]++;
        if (this->counter[node] == this->D) {
            if (this->waiting) {
                for (uint32_t i = 0; i < this->graph_size; i++) {
                    if (this->resource[i]) {
                        this->resource[i] = false;
                        this->waiting = false;
                        this->nodes_with_resource--;
                        break;
                    }
                }
            }
            this->resource[node] = true;
            this->nodes_with_resource++;
            // return 1 + this->D;
            return this->D;
        }
    }

    return 1;
}

uint32_t PageAllocation::write(uint32_t node) {
    uint32_t cost = this->nodes_with_resource;

    if (this->resource[node]) {
        cost -= 1;
    }
    else if (this->counter[node] < this->D && this->waiting) {
        this->counter[node]++;
        if (this->counter[node] == this->D) {
            for (uint32_t i = 0; i < this->graph_size; i++) {
                if (resource[i]) {
                    this->resource[i] = false;
                    break;
                }
            }
            this->waiting = false;
            this->resource[node] = true;
            // cost += this->D;
            cost = this->D;
        }
    }

    if (!this->waiting) {
        for (uint32_t i = 0; i < this->graph_size; i++) {
            if (i != node && this->resource[i]) {
                this->counter[i]--;
                if (this->counter[i] == 0) {
                    if (this->nodes_with_resource > 1) {
                        this->nodes_with_resource--;
                        this->resource[i] = false;
                    }
                    else { // this->nodes_with_resource == 1
                        this->waiting = true;
                    }
                }
            }
        }
    }

    return cost;
}
