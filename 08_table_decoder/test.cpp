#include "lesson.hpp"
#include "../support/test.hpp"

int main() {
    practice::Suite suite;
    suite.run("temperature applies scale and offset", [] {
        lab08::Decoder decoder;
        const auto value = decoder.decode({0x701, {100}});
        CHECK(value && value->metric == lab08::Metric::temperature && value->value == 30.0);
        const auto zero = decoder.decode({0x701, {0}});
        CHECK(zero && zero->value == -20.0);
    });
    suite.run("enabled uses byte one bit six only", [] {
        lab08::Decoder decoder;
        const auto on = decoder.decode({0x702, {0, 0x40}});
        const auto off = decoder.decode({0x702, {0xFF, 0xBF}});
        CHECK(on && on->metric == lab08::Metric::enabled && on->value == 1.0);
        CHECK(off && off->value == 0.0);
    });
    suite.run("mode masks high bits", [] {
        lab08::Decoder decoder;
        const auto value = decoder.decode({0x703, {0xFE}});
        CHECK(value && value->metric == lab08::Metric::mode && value->value == 2.0);
    });
    suite.run("unknown packets increment only unknown count", [] {
        lab08::Decoder decoder;
        CHECK(!decoder.decode({0x7FF, {}}));
        CHECK(!decoder.decode({0x7FF, {1}}));
        CHECK(decoder.unknown_count() == 2 && decoder.short_count() == 0);
    });
    suite.run("short known packets rejected and counted", [] {
        lab08::Decoder decoder;
        CHECK(!decoder.decode({0x701, {}}));
        CHECK(!decoder.decode({0x702, {0x40}}));
        CHECK(!decoder.decode({0x703, {}}));
        CHECK(decoder.short_count() == 3 && decoder.unknown_count() == 0);
    });
    suite.run("extra payload bytes do not alter decoded value", [] {
        lab08::Decoder decoder;
        const auto value = decoder.decode({0x701, {100, 0xFF, 0xFF}});
        CHECK(value && value->value == 30.0);
        CHECK(decoder.short_count() == 0 && decoder.unknown_count() == 0);
    });
    return suite.finish();
}
