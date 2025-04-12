#include <iostream>
#include <random>

typedef struct {
    double approximation;
    double variation;
} experiment_result;

double f(double x);
experiment_result points_in_rectangle(size_t no_of_points, double area_of_square, double exact_solution, std::uniform_real_distribution<double> ux, std::uniform_real_distribution<double> uy, std::mt19937 rng);
experiment_result points_in_axis(size_t no_of_points, double exact_solution, std::uniform_real_distribution<double> ux, std::mt19937 rng);
experiment_result stratified_sampling(size_t no_of_points, double internal_square_area, double rectangle_area, double exact_solution, std::uniform_real_distribution<double> ux, std::uniform_real_distribution<double> uy, std::mt19937 rng);
experiment_result antithetic_variates(size_t no_of_points, double exact_solution, std::uniform_real_distribution<double> ux, std::mt19937 rng);

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

    std::uniform_real_distribution<double> ux_ss(0, sqrt(2.0) / 2.0);
    std::uniform_real_distribution<double> uy_ss(sqrt(2.0) / 2.0, 1.0);

    size_t no_of_points = std::stoul(argv[1]);

    std::cout << "Punkty z prostokąta" << std::endl;
    experiment_result exr = points_in_rectangle(no_of_points / 2, area_of_square, exact_solution, ux, uy, rng);
    std::cout << exr.approximation << std::endl;
    std::cout << exr.variation << std::endl << std::endl;

    std::cout << "Punkty z osi OX" << std::endl;
    exr = points_in_axis(no_of_points, exact_solution, ux, rng);
    std::cout << exr.approximation << std::endl;
    std::cout << exr.variation << std::endl << std::endl;

    std::cout << "Stratified sampling" << std::endl;
    exr = stratified_sampling(no_of_points, 1.0 / 2.0, (sqrt(2.0) / 2.0) * (1.0 - sqrt(2.0) / 2.0), exact_solution, ux_ss, uy_ss, rng);
    std::cout << exr.approximation << std::endl;
    std::cout << exr.variation << std::endl << std::endl;

    std::cout << "Anthetic variates" << std::endl;
    exr = antithetic_variates(no_of_points / 2, exact_solution, ux, rng);
    std::cout << exr.approximation << std::endl;
    std::cout << exr.variation << std::endl << std::endl;

    return 0;
}

double f(double x) {
    return sqrt(1.0 - x * x);
}

experiment_result points_in_rectangle(size_t no_of_points, double area_of_square, double exact_solution, std::uniform_real_distribution<double> ux, std::uniform_real_distribution<double> uy, std::mt19937 rng) {
    size_t no_of_points_under_curve = 0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng), y = uy(rng);
        if (y < f(x)) {
            no_of_points_under_curve++;
        }
    }
    experiment_result exr;
    exr.approximation = (double)no_of_points_under_curve / no_of_points * area_of_square;
    exr.variation = pow(exr.approximation - exact_solution, 2);
    return exr;
}

experiment_result points_in_axis(size_t no_of_points, double exact_solution, std::uniform_real_distribution<double> ux, std::mt19937 rng) {
    double sum_of_values = 0.0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng);
        sum_of_values += f(x);
    }
    experiment_result exr;
    exr.approximation = sum_of_values / no_of_points;
    exr.variation = pow(exr.approximation - exact_solution, 2);
    return exr;
}

experiment_result stratified_sampling(size_t no_of_points, double internal_square_area, double rectangle_area, double exact_solution, std::uniform_real_distribution<double> ux, std::uniform_real_distribution<double> uy, std::mt19937 rng) {
    size_t no_of_points_under_curve = 0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng), y = uy(rng);
        if (y < f(x)) {
            no_of_points_under_curve++;
        }
    }
    experiment_result exr;
    exr.approximation = internal_square_area + 2.0 * rectangle_area * ((double)no_of_points_under_curve / no_of_points);
    exr.variation = pow(exr.approximation - exact_solution, 2);
    return exr;
}

experiment_result antithetic_variates(size_t no_of_points, double exact_solution, std::uniform_real_distribution<double> ux, std::mt19937 rng) {
    double sum_of_values = 0.0;
    for (size_t i = 0; i < no_of_points; i++) {
        double x = ux(rng);
        sum_of_values += f(x);
        sum_of_values += f(1.0 - x);
    }
    experiment_result exr;
    exr.approximation = sum_of_values / (2.0 * no_of_points);
    exr.variation = pow(exr.approximation - exact_solution, 2);
    return exr;
}
