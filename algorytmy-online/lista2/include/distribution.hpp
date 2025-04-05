#ifndef DISTRIBUTION_HPP
#define DISTRIBUTION_HPP

#include <random>

class Distribution {
private:
    static std::random_device dev;
    static std::mt19937 rng;

    static double _harmonic(uint16_t n);
    static double _biharmonic(uint16_t n);

public:
    Distribution() {
        rng.seed(dev());
    }

    static uint16_t uniform(uint16_t min, uint16_t max);
    static uint16_t harmonic(uint16_t n);
    static uint16_t biharmonic(uint16_t n);
    static uint16_t geometric(uint16_t n);
};

#endif // DISTRIBUTION_HPP
