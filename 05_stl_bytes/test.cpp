#include "lesson.hpp"
#include "../support/test.hpp"

int main() {
    practice::Suite suite;
    suite.run("filter includes boundaries and preserves order", [] {
        CHECK(lab05::filter_range({5, -1, 3, 5, 9}, 3, 5) == (std::vector<int>{5, 3, 5}));
    });
    suite.run("empty and reversed ranges", [] {
        CHECK(lab05::filter_range({}, 0, 10).empty());
        CHECK(lab05::filter_range({1, 2}, 3, 1).empty());
    });
    suite.run("first match is not minimum match", [] {
        CHECK(lab05::first_at_least({1, 8, 5}, 5) == 8);
    });
    suite.run("no match is distinct from zero", [] {
        CHECK(!lab05::first_at_least({}, 0));
        CHECK(!lab05::first_at_least({1, 2}, 3));
        CHECK(lab05::first_at_least({0}, 0) == 0);
    });
    suite.run("byte conversion preserves embedded zero", [] {
        CHECK(lab05::to_bytes(std::string("A\0Z", 3)) == (std::vector<std::uint8_t>{0x41, 0, 0x5A}));
        CHECK(lab05::to_bytes("").empty());
    });
    suite.run("byte conversion preserves high byte", [] {
        const std::string text(1, static_cast<char>(0xFF));
        CHECK(lab05::to_bytes(text) == (std::vector<std::uint8_t>{0xFF}));
    });
    suite.run("fixed ASCII is exact length without terminator", [] {
        const auto bytes = lab05::fixed_ascii("DEV7", 4);
        CHECK(bytes && *bytes == (std::vector<std::uint8_t>{0x44, 0x45, 0x56, 0x37}));
        const auto empty = lab05::fixed_ascii("", 0);
        CHECK(empty && empty->empty());
    });
    suite.run("fixed ASCII rejects length and nonprintable bytes", [] {
        CHECK(!lab05::fixed_ascii("ABC", 4));
        CHECK(!lab05::fixed_ascii("ABCDE", 4));
        CHECK(!lab05::fixed_ascii(std::string("A\0B", 3), 3));
        CHECK(!lab05::fixed_ascii(std::string(1, static_cast<char>(0xFF)), 1));
    });
    suite.run("fixed ASCII validates printable boundaries", [] {
        const auto value = lab05::fixed_ascii(" ~", 2);
        CHECK(value && *value == (std::vector<std::uint8_t>{0x20, 0x7E}));
        CHECK(!lab05::fixed_ascii(std::string(1, '\x1F'), 1));
        CHECK(!lab05::fixed_ascii(std::string(1, '\x7F'), 1));
    });
    return suite.finish();
}
