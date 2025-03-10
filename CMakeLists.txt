# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure building the main executable
add_executable(firefly firefly.cpp)

# Configure cmake's testing system
enable_testing()

# Include function used to add regression tests
include(add_regression_test)

# Set paths required for running tests
set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/test_runner.cmake)

# Add tests
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")
