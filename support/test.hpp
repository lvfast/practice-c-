#pragma once
#include "todo.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace practice {
inline void check(bool condition, const char* expression, const char* file, int line) {
    if (!condition) {
        std::ostringstream message;
        message << file << ':' << line << ": " << expression;
        throw std::runtime_error(message.str());
    }
}
class Suite {
public:
    template <typename Function>
    void run(const std::string& name, Function&& test) {
        try {
            std::forward<Function>(test)();
            ++passed_;
            std::cout << "[PASS] " << name << '\n';
        } catch (const Pending& error) {
            ++pending_;
            std::cout << "[TODO] " << name << ": " << error.what() << '\n';
        } catch (const std::exception& error) {
            ++failed_;
            std::cout << "[FAIL] " << name << ": " << error.what() << '\n';
        } catch (...) {
            ++failed_;
            std::cout << "[FAIL] " << name << ": unknown exception\n";
        }
    }
    int finish() const {
        std::cout << "SUMMARY pass=" << passed_ << " fail=" << failed_
                  << " todo=" << pending_ << '\n';
        return failed_ != 0 ? 1 : (pending_ != 0 ? 2 : 0);
    }
private:
    unsigned passed_{0};
    unsigned failed_{0};
    unsigned pending_{0};
};
} // namespace practice

#define CHECK(...) ::practice::check(static_cast<bool>((__VA_ARGS__)), #__VA_ARGS__, __FILE__, __LINE__)
