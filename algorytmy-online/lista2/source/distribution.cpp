#include "distribution.hpp"

double Distribution::_harmonic(uint16_t n) {
    double res = 0.0;
    for (uint16_t i = n; i >= 1; i--) {
        res += 1.0 / (double)(i);
    }
    return res;
}

double Distribution::_biharmonic(uint16_t n) {
    double res = 0.0;
    for (uint16_t i = n; i >= 1; i--) {
        res += 1.0 / (double)(i * i);
    }
    return res;
}

uint16_t Distribution::uniform(uint16_t min, uint16_t max) {
    std::uniform_int_distribution<uint16_t> u(min, max);
    return u(rng);
}

uint16_t Distribution::harmonic(uint16_t n) {
    std::uniform_real_distribution<double> ud(0.0, 1.0);
    double u = ud(rng);
    double hn = _harmonic(n);
    double cumulative = 0.0;
    for (int k = 1; k <= n; k++) {
        cumulative += (1.0 / (k * hn));
        if (u <= cumulative) {
            return k;
        }
    }
    return n;
}

uint16_t Distribution::biharmonic(uint16_t n) {
    std::uniform_real_distribution<double> ud(0.0, 1.0);
    double u = ud(rng);
    double bhn = _biharmonic(n);
    double cumulative = 0.0;
    for (int k = 1; k <= n; k++) {
        cumulative += (1.0 / (k * k * bhn));
        if (u <= cumulative) {
            return k;
        }
    }
    return n;
}

uint16_t Distribution::geometric(uint16_t n) {
    std::bernoulli_distribution bernoulli(0.5);
    for (uint16_t i = 1; i <= n; i++) {
        if (bernoulli(rng)) {
            return i;
        }
    }
    return n;
}
