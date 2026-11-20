#include "lesson.hpp"
#include "../support/test.hpp"
#include <limits>

template <typename T>
void boundaries() {
    const auto zero = lab07::encode_be<T>(T{0});
    for (const auto byte : zero) { CHECK(byte == 0); }
    CHECK(lab07::decode_be<T>(std::vector<std::uint8_t>(sizeof(T), 0)) == T{0});
    const auto maximum = lab07::encode_be<T>(std::numeric_limits<T>::max());
    CHECK(maximum[0] == (std::is_signed_v<T> ? 0x7F : 0xFF));
    for (std::size_t i = 1; i < maximum.size(); ++i) { CHECK(maximum[i] == 0xFF); }
    std::vector<std::uint8_t> max_bytes(sizeof(T), 0xFF);
    max_bytes[0] = std::is_signed_v<T> ? 0x7F : 0xFF;
    CHECK(lab07::decode_be<T>(max_bytes) == std::numeric_limits<T>::max());
    if constexpr (std::is_signed_v<T>) {
        std::vector<std::uint8_t> minimum(sizeof(T), 0);
        minimum[0] = 0x80;
        CHECK(lab07::decode_be<T>(minimum) == std::numeric_limits<T>::min());
        const auto encoded = lab07::encode_be<T>(std::numeric_limits<T>::min());
        CHECK(encoded[0] == 0x80);
        for (std::size_t i = 1; i < encoded.size(); ++i) { CHECK(encoded[i] == 0); }
        const auto minus_one = lab07::encode_be<T>(T{-1});
        for (const auto byte : minus_one) { CHECK(byte == 0xFF); }
        CHECK(lab07::decode_be<T>(std::vector<std::uint8_t>(sizeof(T), 0xFF)) == T{-1});
    }
}
int main() {
    practice::Suite suite;
    suite.run("uint8 boundaries", [] { boundaries<std::uint8_t>(); });
    suite.run("uint16 boundaries", [] { boundaries<std::uint16_t>(); });
    suite.run("uint32 boundaries", [] { boundaries<std::uint32_t>(); });
    suite.run("uint64 boundaries", [] { boundaries<std::uint64_t>(); });
    suite.run("int8 boundaries", [] { boundaries<std::int8_t>(); });
    suite.run("int16 boundaries", [] { boundaries<std::int16_t>(); });
    suite.run("int32 boundaries", [] { boundaries<std::int32_t>(); });
    suite.run("int64 boundaries", [] { boundaries<std::int64_t>(); });
    suite.run("literal big endian encode", [] {
        CHECK(lab07::encode_be<std::uint32_t>(0x12345678U) == (std::array<std::uint8_t, 4>{0x12, 0x34, 0x56, 0x78}));
        CHECK(lab07::encode_be<std::int16_t>(-2) == (std::array<std::uint8_t, 2>{0xFF, 0xFE}));
        CHECK(lab07::encode_be<std::uint64_t>(0x0102030405060708ULL) == (std::array<std::uint8_t, 8>{1, 2, 3, 4, 5, 6, 7, 8}));
    });
    suite.run("literal decode and offset", [] {
        CHECK(lab07::decode_be<std::uint16_t>({0xAA, 0x12, 0x34, 0xBB}, 1) == 0x1234);
        CHECK(lab07::decode_be<std::int16_t>({0xFF, 0xFE}) == -2);
        CHECK(lab07::decode_be<std::uint64_t>({1, 2, 3, 4, 5, 6, 7, 8}) == 0x0102030405060708ULL);
        CHECK(lab07::decode_be<std::int64_t>({0x80, 0, 0, 0, 0, 0, 0, 0}) == std::numeric_limits<std::int64_t>::min());
    });
    suite.run("short and invalid offset", [] {
        CHECK(!lab07::decode_be<std::uint16_t>({}));
        CHECK(!lab07::decode_be<std::uint32_t>({1, 2, 3}));
        CHECK(!lab07::decode_be<std::uint16_t>({1, 2}, 1));
        CHECK(!lab07::decode_be<std::uint8_t>({1}, 1));
        CHECK(!lab07::decode_be<std::uint8_t>({1}, std::numeric_limits<std::size_t>::max()));
    });
    return suite.finish();
}
