#include "lesson.hpp"
#include "../support/test.hpp"
#include <array>

int main() {
    practice::Suite suite;
    suite.run("read first and last byte", [] {
        const std::array<std::uint8_t, 3> data{0x10, 0x00, 0xFF};
        CHECK(lab02::read_byte(data.data(), data.size(), 0) == 0x10);
        CHECK(lab02::read_byte(data.data(), data.size(), 2) == 0xFF);
    });
    suite.run("read null and empty safely", [] {
        CHECK(!lab02::read_byte(nullptr, 3, 0));
        CHECK(!lab02::read_byte(nullptr, 0, 0));
        const std::uint8_t data = 7;
        CHECK(!lab02::read_byte(&data, 0, 0));
    });
    suite.run("read index equal to size is rejected", [] {
        const std::array<std::uint8_t, 2> data{1, 2};
        CHECK(!lab02::read_byte(data.data(), data.size(), 2));
    });
    suite.run("write changes only selected byte", [] {
        std::array<std::uint8_t, 3> data{1, 2, 3};
        CHECK(lab02::write_byte(data.data(), data.size(), 1, 0x80));
        CHECK(data == (std::array<std::uint8_t, 3>{1, 0x80, 3}));
    });
    suite.run("invalid write does not touch buffer", [] {
        std::array<std::uint8_t, 2> data{1, 2};
        CHECK(!lab02::write_byte(data.data(), data.size(), 2, 9));
        CHECK(data == (std::array<std::uint8_t, 2>{1, 2}));
        CHECK(!lab02::write_byte(nullptr, 1, 0, 9));
    });
    suite.run("fill respects supplied subrange", [] {
        std::array<std::uint8_t, 4> data{1, 2, 3, 4};
        CHECK(lab02::fill_bytes(data.data() + 1, 2, 0xAA));
        CHECK(data == (std::array<std::uint8_t, 4>{1, 0xAA, 0xAA, 4}));
    });
    suite.run("zero length fill is a no-op", [] {
        CHECK(lab02::fill_bytes(nullptr, 0, 0));
        CHECK(!lab02::fill_bytes(nullptr, 1, 0));
    });
    return suite.finish();
}
