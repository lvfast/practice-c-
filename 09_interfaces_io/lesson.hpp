#pragma once
#include <cstddef>
#include <fstream>
#include <optional>
#include <ostream>
#include <string>
#include <vector>

namespace lab09 {
struct Record { int sensor_id; double value; };
enum class Status { ok, end, malformed, io_error };
struct ReadResult { Status status; std::optional<Record> record; std::string detail; };
class IRecordSource {
public:
    virtual ~IRecordSource() = default;
    virtual ReadResult next() = 0;
};
class FileSource final : public IRecordSource {
public:
    explicit FileSource(const std::string& path);
    ReadResult next() override;
private:
    std::ifstream input_;
    std::size_t line_number_{0};
};
struct Summary {
    std::size_t accepted{0};
    std::size_t rejected{0};
    double total{0.0};
    bool io_failed{false};
};
Summary collect(IRecordSource& source, std::ostream& log);
} // namespace lab09
