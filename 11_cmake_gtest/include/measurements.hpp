#pragma once
// Supplied library for the build/test exercise; not a Topic 4 implementation.
#include <cstddef>
#include <optional>
#include <vector>

namespace measurements {
struct Sample { int sensor_id; double value; };
struct Statistics { std::size_t count{0}; double sum{0.0}; };
class ISampleSource {
public:
    virtual ~ISampleSource() = default;
    virtual std::optional<Sample> next() = 0;
};
/// Valid iff sensor_id > 0 and value is finite and in [0, 100].
bool is_valid(const Sample& sample);
/// Empty/nonfinite input or any nonfinite intermediate sum returns nullopt.
std::optional<double> average(const std::vector<double>& values);
/// Consume to EOF; invalid samples are skipped.
Statistics summarize(ISampleSource& source);
} // namespace measurements
