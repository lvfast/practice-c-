#include "lesson.hpp"
#include "../support/todo.hpp"
#include <utility>

namespace lab04 {
std::unique_ptr<Device> make_device(int id) { 
    if (id <= 0) return nullptr;
    return std::make_unique<Device>(id); 
}

int borrowed_id(const Device* device) { 
    if (device == nullptr) return -1;
    return device->id();
}

std::unique_ptr<Device> take_device(std::unique_ptr<Device>& source) { 
    return std::move(source);
}

void replace_device(std::unique_ptr<Device>& slot, std::unique_ptr<Device> replacement) { 
    slot = std::move(replacement);
}
} // namespace lab04
