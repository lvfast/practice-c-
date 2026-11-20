#include "lesson.hpp"
#include "../support/test.hpp"
#include <type_traits>
#include <limits>

static_assert(!std::is_copy_constructible_v<lab03::LineReader>, "ifstream owner must not copy");
int main() {
    practice::Suite suite;
    suite.run("sensor state and boundaries", [] {
        lab03::Sensor sensor(8);
        CHECK(sensor.id() == 8 && sensor.value() == 0.0);
        CHECK(sensor.set_value(100.0));
        CHECK(sensor.value() == 100.0);
        CHECK(sensor.set_value(0.0));
        CHECK(sensor.value() == 0.0);
    });
    suite.run("rejected mutation preserves state", [] {
        lab03::Sensor sensor(8);
        CHECK(sensor.set_value(40.0));
        CHECK(!sensor.set_value(-1.0));
        CHECK(sensor.value() == 40.0);
    });
    suite.run("invalid constructor argument", [] {
        bool thrown = false;
        try { const lab03::Sensor sensor(0); }
        catch (const std::invalid_argument&) { thrown = true; }
        CHECK(thrown);
    });
    suite.run("nonfinite sensor update does not mutate state", [] {
        lab03::Sensor sensor(1);
        CHECK(sensor.set_value(25.0));
        CHECK(!sensor.set_value(std::numeric_limits<double>::quiet_NaN()));
        CHECK(!sensor.set_value(std::numeric_limits<double>::infinity()));
        CHECK(sensor.value() == 25.0);
    });
    suite.run("file lines preserve empty line and reach EOF", [] {
        lab03::LineReader reader("data/lines.txt");
        CHECK(reader.next() == "alpha");
        CHECK(reader.next() == "");
        CHECK(reader.next() == "omega");
        CHECK(!reader.next());
        CHECK(!reader.next());
    });
    suite.run("missing file reports an error", [] {
        bool thrown = false;
        try { lab03::LineReader reader("data/does-not-exist.txt"); }
        catch (const practice::Pending&) { throw; }
        catch (const std::runtime_error&) { thrown = true; }
        CHECK(thrown);
    });
    suite.run("a reader can be used again in a new scope", [] {
        { lab03::LineReader reader("data/lines.txt"); CHECK(reader.next() == "alpha"); }
        { lab03::LineReader reader("data/lines.txt"); CHECK(reader.next() == "alpha"); }
    });
    return suite.finish();
}
