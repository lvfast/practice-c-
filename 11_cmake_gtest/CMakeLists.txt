cmake_minimum_required(VERSION 3.20)
project(measurement_practice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Werror)
endif()

# Supplied baseline: library and demo. Explain PUBLIC vs PRIVATE in the README task.
add_library(measurements src/measurements.cpp)
target_include_directories(measurements PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(measurements_demo src/main.cpp)
target_link_libraries(measurements_demo PRIVATE measurements)

include(CTest) # Defines BUILD_TESTING (ON by default) and enables CTest.
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    # TODO 11.1: create measurements_tests from tests/measurements_test.cpp.
    # TODO 11.2: link measurements and GTest::gtest_main to that target.
    # TODO 11.3: include(GoogleTest), then discover the executable's tests.
    # TODO 11.4: remove the warning and the pending test below AFTER doing 11.1-11.3.
    message(WARNING "[TODO] Lesson 11: GoogleTest target/CTest registration are unfinished; see README.md")
    add_test(NAME TODO_register_google_tests COMMAND "${CMAKE_COMMAND}" -E false)
endif()
