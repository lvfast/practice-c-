#include "lesson.hpp"

namespace lab10 {
void LatestValue::update(int) { practice::todo("10: protect snapshot update"); }
std::optional<int> LatestValue::read() const { practice::todo("10: protect const snapshot read"); }
} // namespace lab10
