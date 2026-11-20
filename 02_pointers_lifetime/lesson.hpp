#pragma once
#include <cstddef>
#include <cstdint>
#include <optional>

namespace lab02 {
std::optional<std::uint8_t> read_byte(const std::uint8_t* data, std::size_t size, std::size_t index);
bool write_byte(std::uint8_t* data, std::size_t size, std::size_t index, std::uint8_t value);
bool fill_bytes(std::uint8_t* data, std::size_t size, std::uint8_t value);
} // namespace lab02
