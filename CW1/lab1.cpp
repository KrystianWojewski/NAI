#include <any>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cmath>

using mojamapa_t = std::map<int, int>;
using moja_funkcja = std::function<double (mojamapa_t)>;

void wypisz(mojamapa_t mapa, moja_funkcja fun) {
    using namespace std;
    cout << fun(mapa) << endl;
}

double convert_to_degrees(double x) {
    return x * 3.14159/180;
}

int main(int argc, char **argv) {
    using namespace std;
    vector<string> argumenty(argv, argv + argc);
    mojamapa_t mapa;
    map<string, moja_funkcja> formatery;

    for (int i = 2; i < argumenty.size(); i++) {
//        mapa.insert({{"arg" + to_string(i-1), stoi(argumenty.at(i))}});
        mapa[i-1] = stoi(argumenty.at(i));
    }

    formatery["add"] = [](mojamapa_t map) { return map.at(1) + map.at(2); };
    formatery["mod"] = [](mojamapa_t map) { return map.at(1) %  map.at(2); };
    formatery["sin"] = [](mojamapa_t map) {
        double x = convert_to_degrees(map.at(1));
        return sin(x);
    };

    try{
        auto selected_f = argumenty.at(1);
        wypisz(mapa, formatery.at(selected_f));
    } catch (std::out_of_range aor) {
        cout << "Jestem kodem Krystiana" << endl;
        cout << "Zostalem zrobiony na zajecia do Pana Puzniakowskiego";
    }
    return 0;
}