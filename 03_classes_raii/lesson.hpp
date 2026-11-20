#pragma once
#include <fstream>
#include <optional>
#include <string>

namespace lab03 {
class Sensor {
public:
    explicit Sensor(int id);
    bool set_value(double value);
    // Supplied observers; implement validation and mutation in lesson.cpp.
    int id() const { return id_; }
    double value() const { return value_; }
private:
    int id_;
    double value_{0.0};
};
class LineReader {
public:
    explicit LineReader(const std::string& path);
    std::optional<std::string> next();
    // No custom destructor/copy/move: std::ifstream owns and closes its resource.
private:
    std::ifstream input_;
};
} // namespace lab03
