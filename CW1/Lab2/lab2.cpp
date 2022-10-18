#include <iostream>
#include <vector>
#include <functional>
#include <random>
#include <map>

auto brute_force = [](auto f, auto domain) {
    auto current_p = domain();
    auto best_point = current_p;
    try {
        while (true) {
            if (f(current_p) < f(best_point)) {
                best_point = current_p;
            }
            current_p = domain();
        }
    } catch (std::exception &e) {
    }
    return best_point;
};

using domain_t = std::vector<double>;
std::random_device rd;
std::mt19937 mt_generator(rd());
domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t minimal_d, domain_t maximal_d, int max_iterations) {
    domain_t current_p(minimal_d.size());
    for (int i = 0; i < minimal_d.size(); i++) {
        std::uniform_real_distribution<double> dist(minimal_d[i], maximal_d[i]);
        current_p[i] = dist(mt_generator);
    }
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        domain_t new_p(minimal_d.size());
        for (int i = 0; i < minimal_d.size(); i++) {
            std::uniform_real_distribution<double> dist(-1.0/128.0, 1.0/128.0);
            new_p[i] = current_p[i] + dist(mt_generator);
        }
        if (f(current_p) > f(new_p)) {
            current_p = new_p;
        }
    }
    return current_p;
}

int main(int argc, char **argv) {
    std::map<std::string, std::function<double (double)>> brute_formatery;
    std::map<std::string, std::function<double (std::vector<double>)>> hill_formatery;
    brute_formatery["sphere"] = [](double x) {return x*x;};
    brute_formatery["rastrigin"] = [](double x) {return x * x - 10 * (cos(2 * std::numbers::pi * x) * std::numbers::pi/180);};
    brute_formatery["rosenbrock"] = [](double x) {return 100 * pow((x - (x * x)),2) + pow((1 - x),2);};
    brute_formatery["styblinskiTang"] = [](double x) {return (pow(x,4) - (16 * pow(x, 2)) + 5 * x)/2; };
    double current_sphere_x = std::stod(argv[2]);
    auto sphere_generator = [&]() {
        current_sphere_x+= 1.0/128.0;
        if (current_sphere_x >= std::stod(argv[3])) throw std::invalid_argument("finished");
        return current_sphere_x;
    };
    auto best_point = brute_force(brute_formatery.at(argv[1]), sphere_generator);
    std::cout << "best x = " << best_point << std::endl;

    hill_formatery["sphere"] = [](domain_t x) {return x[0]*x[0];};
    hill_formatery["rastrigin"] = [](domain_t x) {return 10 + (x[0] * x[0] - 10 * (cos(2 * std::numbers::pi * x[0]) * std::numbers::pi/180));};
    hill_formatery["rosenbrock"] = [](domain_t x) {return 100 * pow((x[0] - (x[0]*x[0])),2) + pow((1 - x[0]),2);};
    hill_formatery["styblinskiTang"] = [](domain_t x) {return (pow(x[0],4) - (16 * pow(x[0], 2)) + 5 * x[0])/2; };

//    hill_formatery["beale"] = [](domain_t x) {
//        return pow((1.5 - x[0] + x[0]*x[1]),2) +
//        pow((2.25 - x[0] + (x[0] * pow(x[1],2))),2) +
//        pow((2.625 - x[0] + (x[0] * pow(x[1],3))),2);
//    };
    auto best2 = hill_climbing(hill_formatery.at(argv[1]), {std::stod(argv[2])},{std::stod(argv[3])},10000);
    std::cout << "best x = " << best2[0] << std::endl;
    return 0;
}