#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        lab09::FileSource source("data/records.txt");
        const auto result = lab09::collect(source, std::cerr);
        std::cout << "accepted=" << result.accepted << " rejected=" << result.rejected
                  << " sum=" << result.total << '\n';
        if (result.io_failed) { throw std::runtime_error("source IO failed"); }
    });
}
