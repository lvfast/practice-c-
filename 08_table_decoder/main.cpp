#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        lab08::Decoder decoder;
        const auto value = decoder.decode({0x701, {100}});
        if (!value) { throw std::runtime_error("packet rejected"); }
        std::cout << "temperature=" << value->value << '\n';
    });
}
