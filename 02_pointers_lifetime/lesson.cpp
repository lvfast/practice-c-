#include "lesson.hpp"
#include "../support/todo.hpp"

namespace lab02 {
std::optional<std::uint8_t> read_byte(const std::uint8_t* data, std::size_t size, std::size_t index) {
    // Khong truy cap du lieu neu con tro khong tro den buffer.
    if (data == nullptr) {
        return std::nullopt;
    }

    // Chi so hop le la 0..size-1; dieu kien nay cung loai size == 0.
    if (index >= size) {
        return std::nullopt;
    }

    // Tra ban sao cua byte, khong tra con tro/tham chieu vao buffer.
    return data[index];
}
bool write_byte(std::uint8_t* data, std::size_t size, std::size_t index, std::uint8_t value) {
    if (data == nullptr || index >= size){
        return false;
    }

    data[index] = value;
    return true;

}
bool fill_bytes(std::uint8_t* data, std::size_t size, std::uint8_t value) {
    if (size == 0) {
        return true;
    }
    if (data == nullptr){
        return false;
    }
    for (size_t i = 0; i < size; ++i){
        data[i] = value;
    }

    return true;
}
} // namespace lab02
