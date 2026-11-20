#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        std::uint8_t byte = 0x55;
        if (!lab06::write_bit(byte, 7, true)) { throw std::runtime_error("bad bit index"); }
        std::cout << lab06::hex_string({byte}) << '\n';
        std::cout << "scaled(100)=" << lab06::scaled_value(100) << '\n';
    });
}
