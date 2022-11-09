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

        for (int j = 0; j < 20 ; ++j) {
            uniform_int_distribution<int> uni(0, 1);
            ch.push_back(uni(mt_generator));
        }
        pop.push_back(ch);
    }

    return pop;
}

int convert(long long n) {
    int dec = 0, i = 0, rem;

    while (n!=0) {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }

    return dec;
}

std::pair<double, double> decode(chromosome_t chromosome) {
    std::string x_binary = "";
    std::string y_binary = "";
    double x_decimal = 0.0;
    double y_decimal = 0.0;

    for (int i = 0; i < chromosome.size() / 2; i++) {
        x_binary += std::to_string(chromosome[i]);
    }

    for (int i = chromosome.size() / 2; i < chromosome.size(); i++) {
        y_binary += std::to_string(chromosome[i]);
    }

    x_decimal = (convert(stol(x_binary)) / pow(2.0, (chromosome.size() / 2 - 4)) - 8);
    y_decimal = (convert(stol(y_binary)) / pow(2.0, (chromosome.size() / 2 - 4)) - 8);

    return {x_decimal, y_decimal};
}

auto three_hump_camel_f_v = [](std::pair<double, double> pair) {
    return (2 * pow(pair.first, 2)) - (1.05 * pow(pair.first, 4)) + (pow(pair.first, 6) / 6) + (pair.first * pair.second) + (pow(pair.second, 2));
};

double fintess_function(std::pair<double,double> pop) {
    return 1.0 / (1.0 + abs(three_hump_camel_f_v(pop)));
}

int main() {
    population_t population = generate_population(100 + (23215 % 10) * 2);

    for (auto &chromosome: population) {
        auto decoded = decode(chromosome);
        std::cout << decoded.first << ", " << decoded.second << " | " << fintess_function(decoded) << std::endl;
    }
}