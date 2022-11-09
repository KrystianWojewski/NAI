#include <iostream>
#include <vector>
#include <functional>
#include <random>

std::random_device rd;
std::mt19937 mt_generator(rd());

using chromosome_t = std::vector<int>;
using population_t = std::vector<chromosome_t>;

population_t generate_population(int size) {
    using namespace std;
    population_t pop;

    for (int i = 0; i < size; ++i) {
        chromosome_t ch;

        for (int j = 0; j < 20; ++j) {
            uniform_int_distribution<int> uni(0, 1);
            ch.push_back(uni(mt_generator));
        }
        pop.push_back(ch);
    }

    return pop;
}

std::pair<double, double> decode(chromosome_t chromosome) {
    double x = 0.0;
    double y = 0.0;

    for (int i = 0; i < chromosome.size() / 2; i++) {
        x = x * 2 + chromosome[i];
    }

    for (int i = chromosome.size() / 2; i < chromosome.size(); i++) {
        y = y * 2 + chromosome[i];
    }

    x = x / pow(2.0, (chromosome.size() / 2 - 4)) - 8;
    y = y / pow(2.0, (chromosome.size() / 2 - 4)) - 8;

    return {x, y};
}

auto three_hump_camel_f_v = [](std::pair<double, double> pair) {
    return (2 * pow(pair.first, 2)) - (1.05 * pow(pair.first, 4)) + (pow(pair.first, 6) / 6) +
           (pair.first * pair.second) + (pow(pair.second, 2));
};

double fintess_function(std::pair<double, double> pop) {
    return 1.0 / (1.0 + abs(three_hump_camel_f_v(pop)));
}

int main() {
    population_t population = generate_population(100 + (23215 % 10) * 2);

    for (auto &chromosome: population) {
        auto decoded = decode(chromosome);
        std::cout << decoded.first << ", " << decoded.second << " | " << fintess_function(decoded) << std::endl;
    }
}