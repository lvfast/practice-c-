#include "lesson.hpp"
#include "../support/test.hpp"
#include <cmath>
#include <limits>

int main() {
    practice::Suite suite;
    suite.run("read bit zero and bit seven", [] {
        CHECK(lab06::read_bit(0x81, 0) == true);
        CHECK(lab06::read_bit(0x81, 7) == true);
        CHECK(lab06::read_bit(0x81, 1) == false);
    });
    suite.run("invalid bit index", [] {
        CHECK(!lab06::read_bit(0xFF, 8));
        CHECK(!lab06::read_bit(0xFF, std::numeric_limits<unsigned>::max()));
    });
    suite.run("set and clear preserve other bits", [] {
        std::uint8_t byte = 0x55;
        CHECK(lab06::write_bit(byte, 7, true)); CHECK(byte == 0xD5);
        CHECK(lab06::write_bit(byte, 0, false)); CHECK(byte == 0xD4);
        CHECK(lab06::write_bit(byte, 7, true)); CHECK(byte == 0xD4);
    });
    suite.run("invalid write leaves byte unchanged", [] {
        std::uint8_t byte = 0x55;
        CHECK(!lab06::write_bit(byte, 8, true)); CHECK(byte == 0x55);
    });
    suite.run("extract low middle and full width", [] {
        CHECK(lab06::extract_bits(0xB6, 0, 2) == 2);
        CHECK(lab06::extract_bits(0xB6, 2, 3) == 5);
        CHECK(lab06::extract_bits(0xB6, 0, 8) == 0xB6);
        CHECK(lab06::extract_bits(0x80, 7, 1) == 1);
    });
    suite.run("reject invalid field before shifting", [] {
        CHECK(!lab06::extract_bits(0xFF, 0, 0));
        CHECK(!lab06::extract_bits(0xFF, 7, 2));
        CHECK(!lab06::extract_bits(0xFF, 8, 1));
        CHECK(!lab06::extract_bits(0xFF, std::numeric_limits<unsigned>::max(), 2));
    });
    suite.run("convert unsigned raw measurement", [] {
        CHECK(std::abs(lab06::scaled_value(0) - (-40.0)) < 1e-9);
        CHECK(std::abs(lab06::scaled_value(100) - (-15.0)) < 1e-9);
        CHECK(std::abs(lab06::scaled_value(65535) - 16343.75) < 1e-9);
    });
    suite.run("decode enum from low two bits", [] {
        CHECK(lab06::decode_mode(0xFC) == lab06::Mode::off);
        CHECK(lab06::decode_mode(0xFD) == lab06::Mode::idle);
        CHECK(lab06::decode_mode(0xFE) == lab06::Mode::run);
        CHECK(lab06::decode_mode(0xFF) == lab06::Mode::error);
    });
    suite.run("hex display uses numeric bytes", [] {
        CHECK(lab06::hex_string({0, 0x0A, 0x80, 0xFF}) == "00 0A 80 FF");
        CHECK(lab06::hex_string({}).empty());
    });
    return suite.finish();
}
