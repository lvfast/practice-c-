#include "lesson.hpp"
#include "../support/demo.hpp"
#include <iomanip>

int main() {
    return practice::demo([] {
        const auto bytes = lab07::encode_be<std::uint16_t>(0x1234);
        for (const auto byte : bytes) {
            std::cout << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
                      << static_cast<unsigned>(byte) << ' ';
        }
        std::cout << '\n';
    });
}
