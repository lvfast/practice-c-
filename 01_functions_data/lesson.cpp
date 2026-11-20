#include "lesson.hpp"
#include "../support/todo.hpp"
#include <cmath>

namespace lab01 {

bool is_valid(const Reading& reading){
    return reading.sensor_id > 0
        && std::isfinite(reading.value)
        && reading.value >= 0.0
        && reading.value <= 100.0;
        
}
bool try_update(Reading& reading, double new_val) { 
    if (reading.sensor_id > 0 && std::isfinite(new_val) && new_val >= 0.0 && new_val <= 100.0){
        reading.value = new_val;
        return true;
    } 
    return false;
}
Reading copy_with_offset(Reading reading, double offset) { 
    if (!is_valid(reading)){
        throw std::invalid_argument("Reading ban dau khong hop le");
    } 

    reading.value += offset;

    if (!is_valid(reading)) {
        throw std::invalid_argument("Ket qua sau khi cong khong hop le");
    }

    return reading;
}

} // namespace lab01

