#include "../include/measurements.hpp"
#include <iostream>

int main() {
    const auto result = measurements::average({10.0, 30.0});
    if (!result) { std::cerr << "invalid demo input\n"; return 1; }
    std::cout << "count=2 mean=" << *result << '\n';
}
