#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Provide number of points to be randomly sampled as an arguemnt." << std::endl;
        return 1;
    }

    double min_x = 0.0, max_x = 1.0;
    double min_y = 0, max_y = 1.0;
    double exact_solution = M_PI / 4.0;
    double area_of_square = (max_x - min_x) * (max_y - min_y);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<double> ux(min_x, max_x);
    std::uniform_real_distribution<double> uy(min_y, max_y);

    size_t no_of_points = std::stoul(argv[1]);

    // losowanie punktów na płaszczyźnie
    size_t no_of_points_under_curve = 0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng), y = uy(rng);
        if (y < sqrt(1 - x * x)) {
            no_of_points_under_curve++;
        }
    }
    std::cout << (double)no_of_points_under_curve / no_of_points * area_of_square << std::endl;


    // losowanie punków na osi
    double sum_of_values = 0.0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng);
        sum_of_values += sqrt(1 - x * x);
    }
    std::cout << sum_of_values / no_of_points << std::endl;

    std::cout << exact_solution << std::endl;

    return 0;
}