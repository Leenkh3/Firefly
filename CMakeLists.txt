# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure building the main executable

# Adding Matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
)

# Add ICD executable
add_executable(ICD src/ICD.cpp)  

# Link the Matrix library to the main executable
add_executable(firefly firefly.cpp)

# Configure the link command
# target_link_libraries(firefly PRIVATE GmshMeshReader)

# Configure cmake's testing system
enable_testing()

# Add test for Matrix library
add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Add a test for ICD
add_executable(test_ICD tests/test_ICD.cpp)
target_link_libraries(test_ICD PRIVATE ICD)  

# Add tests
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)

# Add a test for ICD
add_test(NAME ICDUnitTest COMMAND test_ICD)  

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
