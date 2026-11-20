// Offline maintainer check; the assignment's unit-test target uses GoogleTest.
#include "../include/measurements.hpp"
#include "fake_source.hpp"
#include "../../support/test.hpp"
#include <limits>

int main() {
    practice::Suite suite;
    suite.run("provided library validates boundaries and bad inputs", [] {
        CHECK(measurements::is_valid({1, 0.0})); CHECK(measurements::is_valid({1, 100.0}));
        CHECK(!measurements::is_valid({0, 5.0})); CHECK(!measurements::is_valid({1, 101.0}));
        CHECK(!measurements::is_valid({1, std::numeric_limits<double>::quiet_NaN()}));
    });
    suite.run("provided average has independently known result", [] {
        CHECK(measurements::average({10, 20, 30}) == 20.0);
        CHECK(!measurements::average({}));
        CHECK(!measurements::average({std::numeric_limits<double>::infinity()}));
        const auto max = std::numeric_limits<double>::max();
        CHECK(!measurements::average({max, max}));
    });
    suite.run("provided collector skips invalid fake records", [] {
        FakeSource source({{1, 10}, {0, 90}, {2, 30}, {3, 200}});
        const auto result = measurements::summarize(source);
        CHECK(result.count == 2 && result.sum == 40.0);
    });
    suite.run("provided collector handles empty source", [] {
        FakeSource source({});
        const auto result = measurements::summarize(source);
        CHECK(result.count == 0 && result.sum == 0.0);
    });
    return suite.finish();
}
