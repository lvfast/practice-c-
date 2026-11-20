#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        lab01::Reading reading{1, 10.0};
        if (!lab01::try_update(reading, 25.0)) { throw std::runtime_error("update rejected"); }
        const auto copy = lab01::copy_with_offset(reading, 5.0);
        std::cout << "original=" << reading.value << " copy=" << copy.value << '\n';
    });
}
