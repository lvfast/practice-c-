#include "lesson.hpp"
#include "../support/test.hpp"
#include <limits>

int main() {
    practice::Suite suite;
    suite.run("valid reading includes both limits", [] {
        CHECK(lab01::is_valid({1, 0.0}));
        CHECK(lab01::is_valid({9, 100.0}));
    });
    suite.run("reject invalid sensor id", [] { CHECK(!lab01::is_valid({0, 12.0})); });
    suite.run("reject out of range", [] {
        CHECK(!lab01::is_valid({1, -0.1}));
        CHECK(!lab01::is_valid({1, 100.1}));
    });
    suite.run("reject nonfinite values", [] {
        CHECK(!lab01::is_valid({1, std::numeric_limits<double>::quiet_NaN()}));
        CHECK(!lab01::is_valid({1, std::numeric_limits<double>::infinity()}));
    });
    suite.run("reference update changes caller", [] {
        lab01::Reading reading{4, 10.0};
        CHECK(lab01::try_update(reading, 25.0));
        CHECK(reading.sensor_id == 4 && reading.value == 25.0);
    });
    suite.run("failed update leaves data unchanged", [] {
        lab01::Reading reading{4, 10.0};
        CHECK(!lab01::try_update(reading, -1.0));
        CHECK(reading.sensor_id == 4 && reading.value == 10.0);
        lab01::Reading invalid{0, 10.0};
        CHECK(!lab01::try_update(invalid, 5.0));
        CHECK(invalid.value == 10.0);
    });
    suite.run("valid replacement can repair an invalid old value", [] {
        lab01::Reading reading{4, -1.0};
        CHECK(lab01::try_update(reading, 25.0));
        CHECK(reading.sensor_id == 4 && reading.value == 25.0);
    });
    suite.run("nonfinite update leaves both fields unchanged", [] {
        lab01::Reading reading{4, 10.0};
        CHECK(!lab01::try_update(reading, std::numeric_limits<double>::quiet_NaN()));
        CHECK(reading.sensor_id == 4 && reading.value == 10.0);
        CHECK(!lab01::try_update(reading, std::numeric_limits<double>::infinity()));
        CHECK(reading.sensor_id == 4 && reading.value == 10.0);
    });
    suite.run("value parameter leaves original unchanged", [] {
        const lab01::Reading original{3, 10.0};
        const auto changed = lab01::copy_with_offset(original, 5.0);
        CHECK(original.value == 10.0);
        CHECK(changed.sensor_id == 3 && changed.value == 15.0);
    });
    suite.run("invalid adjusted copy throws", [] {
        bool thrown = false;
        try { static_cast<void>(lab01::copy_with_offset({1, 99.0}, 2.0)); }
        catch (const std::invalid_argument&) { thrown = true; }
        CHECK(thrown);
    });
    suite.run("adjusted copy rejects invalid original and nonfinite offset", [] {
        for (const auto original : {lab01::Reading{0, 10}, lab01::Reading{1, -1}}) {
            bool thrown = false;
            try { static_cast<void>(lab01::copy_with_offset(original, 5)); }
            catch (const std::invalid_argument&) { thrown = true; }
            CHECK(thrown);
        }
        bool thrown = false;
        try { static_cast<void>(lab01::copy_with_offset({1, 10}, std::numeric_limits<double>::quiet_NaN())); }
        catch (const std::invalid_argument&) { thrown = true; }
        CHECK(thrown);
    });
    return suite.finish();
}
