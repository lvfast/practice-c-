#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        for (const auto value : lab05::filter_range({5, -1, 3, 9}, 3, 5)) { std::cout << value << ' '; }
        const auto bytes = lab05::to_bytes(std::string("A\0Z", 3));
        std::cout << "\nbytes=" << bytes.size() << '\n';
        for (const auto byte : bytes) { std::cout << static_cast<unsigned>(byte) << ' '; }
        std::cout << '\n';
    });
}
