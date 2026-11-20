#pragma once
#include "todo.hpp"
#include <exception>
#include <iostream>
#include <utility>

namespace practice {
template <typename Function>
int demo(Function&& function) {
    try { std::forward<Function>(function)(); return 0; }
    catch (const Pending& error) { std::cout << "[TODO] " << error.what() << '\n'; return 2; }
    catch (const std::exception& error) { std::cerr << "[FAIL] " << error.what() << '\n'; return 1; }
}
} // namespace practice
