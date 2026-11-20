#include "lesson.hpp"
#include "../support/todo.hpp"
#include <algorithm>

namespace lab05 {
std::vector<int> filter_range(const std::vector<int>& values, int low, int high) 
{   
    std::vector<int> result;

    if (low > high)
    {
        return result;
    }

    for (const auto value : values)
    {
        if (value >= low && value <= high)
        {
            result.push_back(value);
        }
    }
    
    return result;
}
std::optional<int> first_at_least(const std::vector<int>&, int) 
{ practice::todo("05: optional lookup"); 
}
std::vector<std::uint8_t> to_bytes(const std::string&) { practice::todo("05: preserve bytes"); }
std::optional<std::vector<std::uint8_t>> fixed_ascii(const std::string&, std::size_t) {
    practice::todo("05: fixed-width printable ASCII");
}
} // namespace lab05
