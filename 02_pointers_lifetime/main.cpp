#include "lesson.hpp"
#include "../support/demo.hpp"
#include <array>

int main() {
    return practice::demo([] {
        std::array<std::uint8_t, 3> buffer{1, 2, 3};
        if (!lab02::write_byte(buffer.data(), buffer.size(), 1, 42)) { throw std::runtime_error("write failed"); }
        const auto byte = lab02::read_byte(buffer.data(), buffer.size(), 1);
        if (!byte) { throw std::runtime_error("read failed"); }
        std::cout << "byte[1]=" << static_cast<unsigned>(*byte) << '\n';
    });
}
