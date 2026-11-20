#pragma once
#include <memory>

namespace lab04 {
// Supplied entity. virtual destructor also supports the test-only derived probe.
class Device {
public:
    explicit Device(int id) : id_(id) {}
    virtual ~Device() = default;
    int id() const { return id_; }
private:
    int id_;
};
std::unique_ptr<Device> make_device(int id);
int borrowed_id(const Device* device);
std::unique_ptr<Device> take_device(std::unique_ptr<Device>& source);
void replace_device(std::unique_ptr<Device>& slot, std::unique_ptr<Device> replacement);
} // namespace lab04
