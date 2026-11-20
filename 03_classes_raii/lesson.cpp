#include "lesson.hpp"
#include "../support/todo.hpp"
#include <cmath>
#include <stdexcept>

namespace lab03 {
Sensor::Sensor(int id) : id_(id) { 
    if (id <= 0){
        throw std::invalid_argument("id phai lon hon 0");
    }
}
bool Sensor::set_value(double val) { 
    if (!std::isfinite(val) || val < 0 || val > 100){
        return false;
    } 
    value_ = val;
    return true;
}

LineReader::LineReader(const std::string& path) : input_(path) { 
    if (!input_.is_open()){
        throw std::runtime_error("Khong mo duoc file: " + path);
    }
}
std::optional<std::string> LineReader::next() { 
    std::string line;

    if (std::getline(input_, line)){
        return line;
    }

    if (input_.bad() || !input_.eof()){
        throw std::runtime_error("Loi khi doc file");
    }

    return std::nullopt;
}
} // namespace lab03
