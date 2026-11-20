#include "lesson.hpp"
#include "../support/test.hpp"
#include <type_traits>

namespace {
// Lifetime instrumentation belongs in tests, not Device's application API.
class TrackedDevice final : public lab04::Device {
public:
    TrackedDevice(int id, int& destroyed) : Device(id), destroyed_(destroyed) {}
    ~TrackedDevice() override { ++destroyed_; }
private:
    int& destroyed_;
};
}
static_assert(!std::is_copy_constructible_v<std::unique_ptr<lab04::Device>>);
int main() {
    practice::Suite suite;
    suite.run("factory creates valid device", [] {
        const auto device = lab04::make_device(7);
        CHECK(device && device->id() == 7);
    });
    suite.run("factory rejects invalid id", [] { CHECK(!lab04::make_device(0)); CHECK(!lab04::make_device(-1)); });
    suite.run("borrow does not transfer ownership", [] {
        const auto device = std::make_unique<lab04::Device>(5);
        CHECK(lab04::borrowed_id(device.get()) == 5);
        CHECK(device && device->id() == 5);
        CHECK(lab04::borrowed_id(nullptr) == -1);
    });
    suite.run("taking ownership empties source and preserves object", [] {
        int destroyed = 0;
        {
            std::unique_ptr<lab04::Device> source = std::make_unique<TrackedDevice>(3, destroyed);
            const auto* address = source.get();
            auto target = lab04::take_device(source);
            CHECK(!source && target.get() == address);
            CHECK(destroyed == 0);
        }
        CHECK(destroyed == 1);
    });
    suite.run("taking null is safe", [] {
        std::unique_ptr<lab04::Device> source;
        CHECK(!lab04::take_device(source));
    });
    suite.run("replacement destroys old object exactly once", [] {
        int old_destroyed = 0;
        int new_destroyed = 0;
        {
            std::unique_ptr<lab04::Device> slot = std::make_unique<TrackedDevice>(1, old_destroyed);
            std::unique_ptr<lab04::Device> replacement = std::make_unique<TrackedDevice>(2, new_destroyed);
            lab04::replace_device(slot, std::move(replacement));
            CHECK(!replacement && slot && slot->id() == 2);
            CHECK(old_destroyed == 1 && new_destroyed == 0);
        }
        CHECK(old_destroyed == 1 && new_destroyed == 1);
    });
    suite.run("null replacement clears slot", [] {
        int destroyed = 0;
        std::unique_ptr<lab04::Device> slot = std::make_unique<TrackedDevice>(1, destroyed);
        lab04::replace_device(slot, nullptr);
        CHECK(!slot && destroyed == 1);
    });
    return suite.finish();
}
