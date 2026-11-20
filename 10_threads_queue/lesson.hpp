#pragma once
#include "../support/todo.hpp"
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <utility>

namespace lab10 {
template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    bool push(T /*value*/) { practice::todo("10: push under lock and notify"); }
    std::optional<T> pop() { 
        std::unique_lock<std::mutex> lock(mutex_);

        changed_.wait(lock, [this] {
            return !items_.empty() || closed_;
        });
        

        
    }
    void close() { practice::todo("10: close idempotently and wake all"); }
private:
    std::queue<T> items_;
    std::mutex mutex_;
    std::condition_variable changed_;
    bool closed_{false};
};
class LatestValue {
public:
    void update(int value);
    std::optional<int> read() const;
private:
    mutable std::mutex mutex_;
    std::optional<int> value_;
};
} // namespace lab10
