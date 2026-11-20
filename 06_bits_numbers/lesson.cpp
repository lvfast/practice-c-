#include "lesson.hpp"
#include "../support/todo.hpp"
#include <iomanip>
#include <sstream>

namespace lab06 {
std::optional<bool> read_bit(std::uint8_t byte, unsigned index) 
{  
    if (index >= kByteBits)
    {
        return std::nullopt;
    }

    const unsigned shifted = static_cast<unsigned>(byte) >> index;
    return (shifted & 1u) != 0u;
}
bool write_bit(std::uint8_t& byte, unsigned index, bool enabled)
{
    if (index >= kByteBits) {
        return false;
    }

    const unsigned mask = 1u << index;
    const unsigned value = static_cast<unsigned>(byte);

    if (enabled) {
        byte = static_cast<std::uint8_t>(value | mask);
    } else {
        byte = static_cast<std::uint8_t>(value & ~mask);
    }

    return true;
}
std::optional<std::uint8_t> extract_bits(std::uint8_t, unsigned, unsigned) { practice::todo("06: extract_bits"); }
double scaled_value(std::uint16_t) { practice::todo("06: scaling"); }
Mode decode_mode(std::uint8_t byte) 
{
    const unsigned mode = static_cast<unsigned>(byte) & 0x03u;
    return static_cast<Mode>(mode);
}
std::string hex_string(const std::vector<std::uint8_t>&) { practice::todo("06: hex output"); }
} // namespace lab06
