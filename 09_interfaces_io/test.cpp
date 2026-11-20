#include "lesson.hpp"
#include "../support/test.hpp"
#include <sstream>
#include <utility>

namespace {
class FakeSource final : public lab09::IRecordSource {
public:
    explicit FakeSource(std::vector<lab09::ReadResult> results) : results_(std::move(results)) {}
    lab09::ReadResult next() override {
        if (index_ == results_.size()) { return {lab09::Status::end, std::nullopt, ""}; }
        return results_[index_++];
    }
private:
    std::vector<lab09::ReadResult> results_;
    std::size_t index_{0};
};
}
int main() {
    practice::Suite suite;
    suite.run("file parses valid record", [] {
        lab09::FileSource source("data/records.txt");
        const auto result = source.next();
        CHECK(result.status == lab09::Status::ok && result.record);
        CHECK(result.record->sensor_id == 1 && result.record->value == 10.0);
    });
    suite.run("malformed record has context and parsing recovers", [] {
        lab09::FileSource source("data/records.txt");
        static_cast<void>(source.next());
        const auto bad = source.next();
        CHECK(bad.status == lab09::Status::malformed && !bad.record);
        CHECK(bad.detail.find("2") != std::string::npos);
        const auto next = source.next();
        CHECK(next.status == lab09::Status::ok && next.record && next.record->value == 20.0);
    });
    suite.run("extra token nonfinite and blank lines are malformed", [] {
        lab09::FileSource source("data/invalid.txt");
        for (int i = 0; i < 5; ++i) {
            const auto result = source.next();
            CHECK(result.status == lab09::Status::malformed && !result.record && !result.detail.empty());
        }
        CHECK(source.next().status == lab09::Status::end);
        CHECK(source.next().status == lab09::Status::end);
    });
    suite.run("missing file is IO error, not EOF", [] {
        lab09::FileSource source("data/missing.txt");
        const auto result = source.next();
        CHECK(result.status == lab09::Status::io_error && !result.record && !result.detail.empty());
    });
    suite.run("missing number overflow and negative data are rejected", [] {
        lab09::FileSource source("data/numeric_edges.txt");
        for (int i = 0; i < 5; ++i) {
            const auto result = source.next();
            CHECK(result.status == lab09::Status::malformed && !result.record && !result.detail.empty());
        }
    });
    suite.run("whitespace and inclusive measurement limits are accepted", [] {
        lab09::FileSource source("data/numeric_edges.txt");
        for (int i = 0; i < 5; ++i) { static_cast<void>(source.next()); }
        const auto low = source.next();
        const auto high = source.next();
        CHECK(low.status == lab09::Status::ok && low.record && low.record->sensor_id == 6 && low.record->value == 0.0);
        CHECK(high.status == lab09::Status::ok && high.record && high.record->sensor_id == 7 && high.record->value == 100.0);
        CHECK(source.next().status == lab09::Status::end);
    });
    suite.run("collector uses injected source and logs malformed context", [] {
        FakeSource source({
            {lab09::Status::ok, lab09::Record{1, 10.0}, ""},
            {lab09::Status::malformed, std::nullopt, "line 7: bad value"},
            {lab09::Status::ok, lab09::Record{2, 20.0}, ""}
        });
        std::ostringstream log;
        const auto summary = lab09::collect(source, log);
        CHECK(summary.accepted == 2 && summary.rejected == 1 && summary.total == 30.0);
        CHECK(!summary.io_failed && log.str().find("line 7: bad value") != std::string::npos);
    });
    suite.run("collector stops at IO failure", [] {
        FakeSource source({
            {lab09::Status::io_error, std::nullopt, "disk failure"},
            {lab09::Status::ok, lab09::Record{1, 80.0}, ""}
        });
        std::ostringstream log;
        const auto result = lab09::collect(source, log);
        CHECK(result.io_failed && result.accepted == 0 && result.total == 0.0);
        CHECK(log.str().find("disk failure") != std::string::npos);
    });
    suite.run("collector handles empty input", [] {
        FakeSource source({});
        std::ostringstream log;
        const auto result = lab09::collect(source, log);
        CHECK(result.accepted == 0 && result.rejected == 0 && result.total == 0.0 && !result.io_failed);
        CHECK(log.str().empty());
    });
    suite.run("collector reads real mixed file", [] {
        lab09::FileSource source("data/records.txt");
        std::ostringstream log;
        const auto result = lab09::collect(source, log);
        CHECK(result.accepted == 2 && result.rejected == 1 && result.total == 30.0);
        CHECK(!result.io_failed && !log.str().empty());
    });
    return suite.finish();
}
