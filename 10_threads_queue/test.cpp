#include "lesson.hpp"
#include "../support/test.hpp"
#include "../support/thread_test.hpp"
#include <chrono>
#include <future>
#include <memory>

using namespace std::chrono_literals;
int main() {
    practice::Suite suite;
    suite.run("FIFO drain after close and reject later push", [] {
        lab10::ThreadSafeQueue<int> queue;
        CHECK(queue.push(10)); CHECK(queue.push(20));
        queue.close(); queue.close();
        CHECK(!queue.push(30));
        CHECK(queue.pop() == 10); CHECK(queue.pop() == 20); CHECK(!queue.pop());
    });
    suite.run("closing empty queue ends pop", [] {
        lab10::ThreadSafeQueue<int> queue;
        queue.close(); CHECK(!queue.pop()); CHECK(!queue.pop());
    });
    suite.run("queue supports move-only payload", [] {
        lab10::ThreadSafeQueue<std::unique_ptr<int>> queue;
        auto value = std::make_unique<int>(42);
        const auto* address = value.get();
        CHECK(queue.push(std::move(value))); CHECK(!value);
        queue.close();
        const auto result = queue.pop();
        CHECK(result && result->get() == address && **result == 42);
        CHECK(!queue.pop());
    });
    suite.run("close wakes waiting consumer", [] {
        lab10::ThreadSafeQueue<int> probe; probe.close(); // TODO must not escape a live thread.
        lab10::ThreadSafeQueue<int> queue;
        std::promise<void> started;
        auto entered = started.get_future();
        std::packaged_task<std::optional<int>()> task([&] {
            started.set_value();
            return queue.pop();
        });
        auto result = task.get_future();
        practice::JoiningThread worker(std::move(task), [&] { queue.close(); });
        entered.wait();
        CHECK(result.wait_for(20ms) == std::future_status::timeout);
        queue.close();
        CHECK(result.wait_for(1s) == std::future_status::ready);
        CHECK(!result.get());
        worker.join();
    });
    suite.run("push wakes consumer with the correct value", [] {
        lab10::ThreadSafeQueue<int> probe; probe.close();
        lab10::ThreadSafeQueue<int> queue;
        std::promise<void> started;
        auto entered = started.get_future();
        std::packaged_task<std::optional<int>()> task([&] { started.set_value(); return queue.pop(); });
        auto result = task.get_future();
        practice::JoiningThread worker(std::move(task), [&] { queue.close(); });
        entered.wait();
        CHECK(result.wait_for(20ms) == std::future_status::timeout);
        CHECK(queue.push(73));
        CHECK(result.wait_for(1s) == std::future_status::ready);
        CHECK(result.get() == 73);
        queue.close(); worker.join();
    });
    suite.run("close wakes every consumer", [] {
        lab10::ThreadSafeQueue<int> probe; probe.close();
        lab10::ThreadSafeQueue<int> queue;
        std::promise<void> started_a;
        std::promise<void> started_b;
        auto entered_a = started_a.get_future();
        auto entered_b = started_b.get_future();
        std::packaged_task<std::optional<int>()> task_a([&] { started_a.set_value(); return queue.pop(); });
        std::packaged_task<std::optional<int>()> task_b([&] { started_b.set_value(); return queue.pop(); });
        auto result_a = task_a.get_future(); auto result_b = task_b.get_future();
        practice::JoiningThread worker_a(std::move(task_a), [&] { queue.close(); });
        practice::JoiningThread worker_b(std::move(task_b), [&] { queue.close(); });
        entered_a.wait(); entered_b.wait();
        CHECK(result_a.wait_for(20ms) == std::future_status::timeout);
        CHECK(result_b.wait_for(20ms) == std::future_status::timeout);
        queue.close();
        CHECK(result_a.wait_for(1s) == std::future_status::ready);
        CHECK(result_b.wait_for(1s) == std::future_status::ready);
        CHECK(!result_a.get() && !result_b.get());
        worker_a.join(); worker_b.join();
    });
    suite.run("null unique_ptr is a payload, not end of queue", [] {
        lab10::ThreadSafeQueue<std::unique_ptr<int>> queue;
        CHECK(queue.push(nullptr)); queue.close();
        const auto value = queue.pop();
        CHECK(value.has_value() && !*value);
        CHECK(!queue.pop());
    });
    suite.run("producer consumer preserves all 500 items in order", [] {
        lab10::ThreadSafeQueue<int> probe; probe.close();
        lab10::ThreadSafeQueue<int> queue;
        std::packaged_task<std::vector<int>()> consume([&] {
            std::vector<int> items;
            while (auto value = queue.pop()) { items.push_back(*value); }
            return items;
        });
        auto consumed = consume.get_future();
        practice::JoiningThread worker(std::move(consume), [&] { queue.close(); });
        for (int i = 0; i < 500; ++i) { CHECK(queue.push(i)); }
        queue.close();
        CHECK(consumed.wait_for(1s) == std::future_status::ready);
        const auto values = consumed.get();
        worker.join();
        CHECK(values.size() == 500);
        for (int i = 0; i < 500; ++i) { CHECK(values[static_cast<std::size_t>(i)] == i); }
    });
    suite.run("latest value distinguishes empty and zero", [] {
        lab10::LatestValue latest;
        CHECK(!latest.read()); latest.update(0); CHECK(latest.read() == 0);
        latest.update(17); CHECK(latest.read() == 17);
    });
    suite.run("concurrent snapshot access", [] {
        lab10::LatestValue latest;
        latest.update(0); CHECK(latest.read() == 0); // Check TODOs before starting workers.
        std::promise<void> release;
        const auto gate = release.get_future().share();
        std::packaged_task<void()> writer([&] {
            gate.wait();
            for (int i = 1; i <= 1000; ++i) { latest.update(i); }
        });
        std::packaged_task<void()> reader([&] {
            gate.wait();
            int previous = 0;
            for (int i = 0; i < 1000; ++i) {
                const auto value = latest.read();
                CHECK(value && *value >= previous && *value <= 1000);
                previous = *value;
            }
        });
        auto written = writer.get_future(); auto read = reader.get_future();
        practice::JoiningThread writer_thread(std::move(writer), [&] {
            // If creating reader_thread throws, release writer before its destructor joins.
            if (gate.wait_for(0ms) != std::future_status::ready) { release.set_value(); }
        });
        practice::JoiningThread reader_thread(std::move(reader));
        release.set_value();
        writer_thread.join(); reader_thread.join();
        written.get(); read.get(); CHECK(latest.read() == 1000);
    });
    return suite.finish();
}
