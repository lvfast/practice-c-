#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace lab08 {
enum class Metric { temperature, enabled, mode };
struct Packet { std::uint16_t code; std::vector<std::uint8_t> payload; };
struct Definition {
    std::uint16_t code;
    Metric metric;
    std::size_t byte_offset;
    unsigned start_bit;
    unsigned width;
    double scale;
    double offset;
};
// TODO 08: replace the zero-initialized rows using the README table.
inline constexpr std::array<Definition, 3> definitions{};
struct Measurement { Metric metric; double value; };
class Decoder {
public:
    std::optional<Measurement> decode(const Packet& packet);
    std::size_t unknown_count() const { return unknown_; }
    std::size_t short_count() const { return short_; }
private:
    std::size_t unknown_{0};
    std::size_t short_{0};
};
} // namespace lab08
