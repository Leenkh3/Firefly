# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Adding Matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
)

# Add ICD library (change ICD from executable to library)
add_library(ICDLib src/ICD.cpp)

# Add firefly executable
add_executable(firefly firefly.cpp)

# Link the Matrix and ICD libraries to the main executable
target_link_libraries(firefly PRIVATE MatrixLib ICDLib)

# Configure cmake's testing system
enable_testing()

# Add test for Matrix library
add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Add a test for ICD (link it to the ICDLib library)
add_executable(test_ICD src/tests/test_ICD.cpp)
target_link_libraries(test_ICD PRIVATE ICDLib)

# Add tests
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME ICDUnitTest COMMAND test_ICD)

# Include function used to add regression tests
include(add_regression_test)

# Set paths required for running tests
set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/test_runner.cmake)

# Add regression tests
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

