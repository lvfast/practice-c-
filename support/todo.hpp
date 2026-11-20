#pragma once
#include <stdexcept>
#include <string>

namespace practice {
class Pending final : public std::logic_error {
public:
    explicit Pending(const std::string& task) : std::logic_error(task) {}
};
[[noreturn]] inline void todo(const std::string& task) { throw Pending(task); }
} // namespace practice
