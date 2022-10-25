#include <functional>
#include <iostream>
#include <list>
#include <random>
#include <vector>

std::mt19937 mt_generator((std::random_device()) ());

using domain_t = std::vector<double>;

std::ostream &operator<<(std::ostream &o, domain_t &d) {
    o << d[0] << " " << d[1];
    return o;
}

domain_t hill_climbing(const std::function<double(domain_t)> &f, domain_t start_point,
                       std::function<std::vector<domain_t>(domain_t)> get_close_points, int max_iterations) {
    domain_t best_p = start_point;
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto close_points = get_close_points(best_p);
        auto best_neighbour = *std::min_element(close_points.begin(), close_points.end(),
                                                [f](auto a, auto b) { return f(a) < f(b); });
        if (f(best_neighbour) < f(best_p)) {
            best_p = best_neighbour;
        }
    }
    return best_p;
}

domain_t tabu_method(const std::function<double(domain_t)> &f, domain_t start_point,
                     std::function<std::vector<domain_t>(domain_t)> get_close_points, int max_iterations) {
    using namespace std;
    domain_t best_point = start_point;
    list<domain_t> tabu_list = {start_point};
    for (int iteration = 0; iteration < max_iterations; iteration++) {
//        cout << iteration << " " << tabu_list.back() << " " << f(tabu_list.back())
//             << endl;
        for (auto tabu_i = tabu_list.rbegin(); tabu_i != tabu_list.rend();
             tabu_i++) {
            auto close_points_all = get_close_points(*tabu_i);
            vector<domain_t> close_points;
            copy_if(close_points_all.begin(), close_points_all.end(),
                    back_inserter(close_points), [&](auto p) {
                        return !count(tabu_list.begin(), tabu_list.end(), p);
                    });
            if (close_points.size() != 0) {
                tabu_list.push_back(*min_element(close_points.begin(), close_points.end(),
                                                 [f](auto a, auto b) { return f(a) < f(b); }));
                break;
            }
        }
        if (f(best_point) > f(tabu_list.back())) best_point = tabu_list.back();
    }
    return best_point;
}

domain_t simulated_annealing(const std::function<double(domain_t)> &f, domain_t start_point,
                             std::function<std::vector<domain_t>(domain_t)> get_close_points, int max_iterations) {
    using namespace std;
    domain_t s = start_point;
    list<domain_t> annealing_list = {s};
    for (int k = 1; k < max_iterations; k++) {
        auto T = k;
        auto annealing_i = annealing_list.begin();
        if (annealing_list.size() > 1) {
            advance(annealing_i, annealing_list.size() - 2);
        }
        {
            auto close_points = get_close_points(*annealing_i);
            auto dupa = get_close_points(s);
            auto t = *std::min_element(close_points.begin(), close_points.end(),
                                       [f](auto a, auto b) { return f(a) < f(b); });
            std::uniform_real_distribution<double> distr(0, 1);
            double uk = distr(mt_generator);
            auto func = exp(-1 * (abs(f(t) - f(*annealing_i)) / T));

            if (f(t) >= f(*annealing_i)) {
                s = t;
                annealing_list.push_back(s);
            } else if (uk < func) {
                s = t;
                annealing_list.push_back(s);
            } else {
                s = *annealing_i;
            }
        }
    }
    return s;
}

int main() {
    auto rastrigin_f_v = [](domain_t x) {
        const double A = 10.0;
        double ret = A * x.size();
        for (int i = 0; i < x.size(); i++) {
            ret = ret + x[i] * x[i] - A * std::cos(2 * M_PI * x[i]);
        }
        return ret;
    };

    auto holder_table_f_v = [](domain_t x) {
        return -abs(sin(x[0]) * cos(x[1]) * exp(abs(1 - (sqrt(pow(x[0], 2) + pow(x[1], 2))) / std::numbers::pi)));
    };

    auto booth_f_v = [](domain_t x) { return pow((x[0] + 2 * x[1] - 7), 2) + pow((2 * x[0] + x[1] - 5), 2); };

    auto three_hump_camel_f_v = [](domain_t x) {
        return (2 * pow(x[0], 2)) - (1.05 * pow(x[0], 4)) + (pow(x[0], 6) / 6) + (x[0] * x[1]) + (pow(x[1], 2));
    };

    auto get_random_point = []() -> domain_t {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {distr(mt_generator), distr(mt_generator)};
    };

    auto get_close_points_random = [](domain_t p0) -> std::vector<domain_t> {
        std::uniform_real_distribution<double> distr(-10, 10);
        return {{distr(mt_generator), distr(mt_generator)}};
    };

    auto get_close_points = [](domain_t p0) -> std::vector<domain_t> {
        std::vector<domain_t> ret;
        for (int i = 0; i < p0.size(); i++) {
            domain_t v = p0;
            v[i] += 1.0 / 64.0;
            ret.push_back(v);
            v = p0;
            v[i] -= 1.0 / 64.0;
            ret.push_back(v);
        }
        return ret;
    };

    auto best0 = tabu_method(holder_table_f_v, get_random_point(), get_close_points, 10000);
    std::cout << "# tabu x = " << best0[0] << " " << best0[1] << std::endl;
    auto best1 = hill_climbing(booth_f_v, get_random_point(), get_close_points, 100000);
    std::cout << "# hill x = " << best1[0] << " " << best1[1] << std::endl;
    auto best2 = simulated_annealing(booth_f_v, get_random_point(), get_close_points, 100000);
    std::cout << "# annealing x = " << best2[0] << " " << best2[1] << std::endl;
    return 0;
}