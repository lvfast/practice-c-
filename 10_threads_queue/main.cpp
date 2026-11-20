#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        // Start with this deterministic queue demo; test.cpp exercises live workers.
        lab10::ThreadSafeQueue<int> queue;
        if (!queue.push(21)) { throw std::runtime_error("queue closed too early"); }
        queue.close();
        const auto value = queue.pop();
        if (!value) { throw std::runtime_error("lost queue item"); }
        lab10::LatestValue latest;
        latest.update(*value);
        const auto snapshot = latest.read();
        if (!snapshot) { throw std::runtime_error("missing snapshot"); }
        std::cout << "received=" << *snapshot << '\n';
        std::cout << "Run test mode for producer/consumer and wake-up scenarios.\n";
    });
}
