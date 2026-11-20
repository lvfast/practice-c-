#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace lab05 {
std::vector<int> filter_range(const std::vector<int>& values, int low, int high);
std::optional<int> first_at_least(const std::vector<int>& values, int threshold);
std::vector<std::uint8_t> to_bytes(const std::string& text);
std::optional<std::vector<std::uint8_t>> fixed_ascii(const std::string& text, std::size_t width);
} // namespace lab05
