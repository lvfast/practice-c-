#include "lesson.hpp"
#include "../support/demo.hpp"

int main() {
    return practice::demo([] {
        lab03::Sensor sensor(7);
        if (!sensor.set_value(45.0)) { throw std::runtime_error("invalid value"); }
        std::cout << "sensor=" << sensor.id() << " value=" << sensor.value() << '\n';
        {
            lab03::LineReader reader("data/lines.txt");
            while (const auto line = reader.next()) { std::cout << '[' << *line << "]\n"; }
        }
        std::cout << "reader scope ended; its ifstream has been destroyed\n";
    });
}
