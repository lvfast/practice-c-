#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace lab06 {
inline constexpr unsigned kByteBits = 8;
inline constexpr double kScale = 0.25;
inline constexpr double kOffset = -40.0;
enum class Mode : std::uint8_t { off = 0, idle = 1, run = 2, error = 3 };
std::optional<bool> read_bit(std::uint8_t byte, unsigned index);
bool write_bit(std::uint8_t& byte, unsigned index, bool enabled);
std::optional<std::uint8_t> extract_bits(std::uint8_t byte, unsigned start, unsigned width);
double scaled_value(std::uint16_t raw);
Mode decode_mode(std::uint8_t byte);
std::string hex_string(const std::vector<std::uint8_t>& bytes);
} // namespace lab06
