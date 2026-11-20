#pragma once
#include "../support/todo.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <type_traits>
#include <vector>

namespace lab07 {
template <typename T>
std::array<std::uint8_t, sizeof(T)> encode_be(T /*value*/) {
    // Supplied constraint; implement the byte conversion below it.
    static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>,
                  "encode_be requires an integer type other than bool");
    practice::todo("07: encode_be<T>");
}
template <typename T>
std::optional<T> decode_be(const std::vector<std::uint8_t>& /*bytes*/, std::size_t /*offset*/ = 0) {
    static_assert(std::is_integral_v<T> && !std::is_same_v<std::remove_cv_t<T>, bool>,
                  "decode_be requires an integer type other than bool");
    practice::todo("07: decode_be<T>");
}
} // namespace lab07
