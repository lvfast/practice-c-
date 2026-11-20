#include "../include/measurements.hpp"
#include <cmath>

namespace measurements {
bool is_valid(const Sample& sample) {
    return sample.sensor_id > 0 && std::isfinite(sample.value)
        && sample.value >= 0.0 && sample.value <= 100.0;
}
std::optional<double> average(const std::vector<double>& values) {
    if (values.empty()) { return std::nullopt; }
    double sum = 0.0;
    for (const auto value : values) {
        if (!std::isfinite(value)) { return std::nullopt; }
        sum += value;
        if (!std::isfinite(sum)) { return std::nullopt; }
    }
    return sum / static_cast<double>(values.size());
}
Statistics summarize(ISampleSource& source) {
    Statistics result;
    while (const auto sample = source.next()) {
        if (is_valid(*sample)) { ++result.count; result.sum += sample->value; }
    }
    return result;
}
} // namespace measurements
