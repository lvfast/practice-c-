// Infrastructure regression: simulate failure creating a second worker.
#include "test.hpp"
#include "thread_test.hpp"
#include <chrono>
#include <future>
#include <system_error>

int main() {
    practice::Suite suite;
    suite.run("unwinding releases start gate before joining", [] {
        std::promise<void> release;
        const auto gate = release.get_future().share();
        std::packaged_task<void()> task([&] { gate.wait(); });
        auto finished = task.get_future();
        try {
            practice::JoiningThread writer(std::move(task), [&] {
                if (gate.wait_for(std::chrono::milliseconds(0)) != std::future_status::ready) {
                    release.set_value();
                }
            });
            throw std::system_error(std::make_error_code(std::errc::resource_unavailable_try_again));
        } catch (const std::system_error&) {}
        CHECK(finished.wait_for(std::chrono::seconds(1)) == std::future_status::ready);
        finished.get();
    });
    return suite.finish();
}
