// Infrastructure test only; not an exercise solution.
#include "test.hpp"
#include <string>

int main(int argc, char** argv) {
    const std::string mode = argc > 1 ? argv[1] : "pass";
    practice::Suite suite;
    suite.run("runner contract", [&] {
        if (mode == "todo") { practice::todo("self-test"); }
        if (mode == "exception") { throw std::runtime_error("unexpected"); }
        CHECK(mode == "pass");
    });
    return suite.finish();
}
