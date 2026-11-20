#include "lesson.hpp"
#include "../support/todo.hpp"
#include <cmath>
#include <sstream>

namespace lab09 {
FileSource::FileSource(const std::string&) { practice::todo("09: open source without conflating IO error and EOF"); }
ReadResult FileSource::next() { practice::todo("09: parse one line and classify result"); }
Summary collect(IRecordSource&, std::ostream&) { practice::todo("09: consume abstract source and log errors"); }
} // namespace lab09
