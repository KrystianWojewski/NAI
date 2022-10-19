#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>

std::mt19937 mt_generator((std::random_device())());

using domain_t = std::vector<double>;

domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t start_point, std::function<std::vector<domain_t>()> get_close_points, int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points();
        auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(), [f](auto a, auto b){return f(a) > f(b);});
        if (f(best_neighbour) < f(best_p)) best_p = best_neighbour;
    }
    return best_p;
}

int main(int argc, char **argv) {
    std::map<std::string, std::function<double (std::vector<double>)>> hill_formatery;

    hill_formatery["himmelblau"] = [](domain_t x) {return pow((x[0] * x[0] + x[1] - 11), 2) + pow((x[0] + x[1] * x[1] -7), 2);};
    hill_formatery["booth"] = [](domain_t x) {return pow((x[0] + 2*x[1] - 7), 2) + pow((2*x[0] + x[1] -5), 2);};
    hill_formatery["beale"] = [](domain_t x) {
        return pow((1.5 - x[0] + x[0]*x[1]),2) +
        pow((2.25 - x[0] + (x[0] * pow(x[1],2))),2) +
        pow((2.625 - x[0] + (x[0] * pow(x[1],3))),2);
    };

    auto get_random_point = [=]() -> domain_t {
        std::uniform_real_distribution<double>distr(std::stod(argv[2]), std::stod(argv[3]));
        return {distr(mt_generator), distr(mt_generator)};
    };
    auto get_close_points_random = [=]() -> std::vector<domain_t> {
        std::uniform_real_distribution<double>distr(std::stod(argv[2]),std::stod(argv[3]));
        return {{distr(mt_generator), distr(mt_generator)}};
    };

    auto best1 = hill_climbing(hill_formatery.at(argv[1]), get_random_point(), get_close_points_random,1000000);
    std::cout << "hill_climbing x = " << best1[0] << std::endl;
    std::cout << "hill_climbing y = " << best1[1] << std::endl;

    return 0;
}