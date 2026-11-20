#pragma once
// Test infrastructure only. Cleanup closes a queue before joining on exceptions.
#include <functional>
#include <thread>
#include <utility>

namespace practice {
class JoiningThread {
public:
    template <typename Function>
    explicit JoiningThread(Function&& function, std::function<void()> cleanup = {})
        : cleanup_(std::move(cleanup)), thread_(std::forward<Function>(function)) {}
    ~JoiningThread() {
        if (cleanup_) { try { cleanup_(); } catch (...) {} }
        join();
    }
    JoiningThread(const JoiningThread&) = delete;
    JoiningThread& operator=(const JoiningThread&) = delete;
    void join() { if (thread_.joinable()) { thread_.join(); } }
private:
    std::function<void()> cleanup_;
    std::thread thread_;
};
} // namespace practice
