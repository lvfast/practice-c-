#include "measurements.hpp"
#include "fake_source.hpp"
#include <gtest/gtest.h>
#include <limits>

TEST(Validation, InclusiveBoundaries) {
    EXPECT_TRUE(measurements::is_valid({1, 0.0}));
    EXPECT_TRUE(measurements::is_valid({1, 100.0}));
}
TEST(Validation, InvalidId) { EXPECT_FALSE(measurements::is_valid({0, 10.0})); }
TEST(Validation, OutOfRange) {
    EXPECT_FALSE(measurements::is_valid({1, -1.0}));
    EXPECT_FALSE(measurements::is_valid({1, 101.0}));
}
TEST(Validation, Nonfinite) {
    EXPECT_FALSE(measurements::is_valid({1, std::numeric_limits<double>::quiet_NaN()}));
    EXPECT_FALSE(measurements::is_valid({1, std::numeric_limits<double>::infinity()}));
}
TEST(Average, Empty) { EXPECT_FALSE(measurements::average({})); }
TEST(Average, LiteralExpectedValue) {
    const auto value = measurements::average({10.0, 20.0, 30.0});
    ASSERT_TRUE(value); EXPECT_DOUBLE_EQ(*value, 20.0);
}
TEST(Average, RejectsNonfinite) {
    EXPECT_FALSE(measurements::average({10.0, std::numeric_limits<double>::infinity()}));
    EXPECT_FALSE(measurements::average({std::numeric_limits<double>::quiet_NaN()}));
}
TEST(Average, RejectsOverflowedSum) {
    const auto largest = std::numeric_limits<double>::max();
    EXPECT_FALSE(measurements::average({largest, largest}));
}
TEST(Summary, ConsumesInjectedSource) {
    FakeSource source({{1, 10.0}, {2, 30.0}});
    const auto result = measurements::summarize(source);
    EXPECT_EQ(result.count, 2U); EXPECT_DOUBLE_EQ(result.sum, 40.0);
}
TEST(Summary, SkipsInvalidRecords) {
    FakeSource source({{1, 10.0}, {0, 99.0}, {2, 200.0}, {3, 30.0}});
    const auto result = measurements::summarize(source);
    EXPECT_EQ(result.count, 2U); EXPECT_DOUBLE_EQ(result.sum, 40.0);
}
TEST(Summary, EmptySource) {
    FakeSource source({});
    const auto result = measurements::summarize(source);
    EXPECT_EQ(result.count, 0U); EXPECT_DOUBLE_EQ(result.sum, 0.0);
}
