#include <iostream>
#include <random>
#include <math.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Provide number of points to be randomly sampled as an arguemnt." << std::endl;
        return 1;
    }

    double min_x = 0.0, max_x = M_PI;
    double min_y = 0, max_y = 1.0;
    double exact_solution = 2.0;
    double area_of_square = (max_x - min_x) * (max_y - min_y);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> ux(min_x, max_x);
    std::uniform_real_distribution<double> uy(min_y, max_y);

    size_t no_of_points = std::stoul(argv[1]);
    size_t no_of_points_under_curve = 0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng), y = uy(rng);
        if (y <= sin(x)) {
            no_of_points_under_curve++;
        }
    }

    double approximation = (double)no_of_points_under_curve / (double)no_of_points * area_of_square;
    std::cout << approximation << ";";

    double relative_error = abs(exact_solution - approximation) / exact_solution;
    std::cout << relative_error << std::endl;

    return 0;
}