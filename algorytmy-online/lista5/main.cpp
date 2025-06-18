#include <iostream>
#include <random>

#include "page-allocation.hpp"

int main() {
    const uint32_t graph_size = 64;
    const uint32_t request_seq_len = 65536;
    const uint32_t d_values[] = { 16, 32, 64, 128, 256 };
    const double p_values[] = { 0.01, 0.02, 0.05, 0.1, 0.2, 0.5 };
    const uint32_t no_of_repetitions = 1000;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<uint32_t> uniform_int(0, graph_size - 1);
    std::uniform_real_distribution<double> uniform_double(0, 1);

    std::cout << "D;p;avg-copies;max-copies;cost" << std::endl;
    for (uint32_t D : d_values) {
        for (double p : p_values) {
            uint64_t avg_cost = 0;
            uint32_t avg_resource_copies = 0;
            uint32_t max_resource_copies = 0;
            for (uint32_t i = 0; i < no_of_repetitions; i++) {
                PageAllocation pa(graph_size, D);
                for (uint32_t j = 0; j < request_seq_len; j++) {
                    if (uniform_double(rng) < p) {
                        avg_cost += (uint64_t)pa.write(uniform_int(rng));
                    }
                    else {
                        avg_cost += (uint64_t)pa.read(uniform_int(rng));
                    }
                    uint32_t resource_copies = pa.get_no_of_resource_copies();
                    avg_resource_copies += resource_copies;
                    if (resource_copies > max_resource_copies) {
                        max_resource_copies = resource_copies;
                    }
                }
            }
            std::cout << D << ";" << p << ";" << (double)avg_resource_copies / no_of_repetitions / request_seq_len << ";" << max_resource_copies << ";" << (double)avg_cost / no_of_repetitions / request_seq_len << std::endl;
        }
    }

    return 0;
}