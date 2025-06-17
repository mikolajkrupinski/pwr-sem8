#include <iostream>
#include <random>
#include <vector>

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    {
        std::vector<size_t> moments1;
        size_t n = 0;
        while (moments1.size() < 20) {
            n++;
            if (dist(gen) < 1.0 / n) {
                moments1.push_back(n);
            }
        }

        for (size_t i : moments1) {
            std::cout << i << " ";
        }
        std::cout << std::endl << std::endl;
    }

    {
        std::vector<size_t> moments2;
        size_t n = 1;
        moments2.push_back(1);
        while (moments2.size() < 50) {
            double u = dist(gen);
            size_t next = n + (size_t)ceil((u * n) / (1 - u));
            moments2.push_back(next);
            n = next;
        }

        for (size_t i : moments2) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}