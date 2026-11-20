#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        auto source = lab04::make_device(7);
        auto owner = lab04::take_device(source);
        if (!owner) { throw std::runtime_error("missing owner"); }
        std::cout << "source empty=" << std::boolalpha << !source
                  << " borrowed id=" << lab04::borrowed_id(owner.get()) << '\n';
    });
}
