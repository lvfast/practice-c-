#pragma once
#include "../include/measurements.hpp"
#include <utility>
#include <vector>

// Supplied fake: only supplies input; assertions target summarize(), not the fake.
class FakeSource final : public measurements::ISampleSource {
public:
    explicit FakeSource(std::vector<measurements::Sample> values) : values_(std::move(values)) {}
    std::optional<measurements::Sample> next() override {
        if (index_ == values_.size()) { return std::nullopt; }
        return values_[index_++];
    }
private:
    std::vector<measurements::Sample> values_;
    std::size_t index_{0};
};
