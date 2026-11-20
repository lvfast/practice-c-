#pragma once
namespace lab01 {
struct Reading { int sensor_id; double value; };
bool is_valid(const Reading& reading);
bool try_update(Reading& reading, double value);
Reading copy_with_offset(Reading reading, double offset);
} // namespace lab01
