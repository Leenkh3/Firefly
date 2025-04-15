# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name, version, and languages ​​used
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set the path of the additional modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add source directory to main project
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Library for dealing with arrays
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
)

#CSR Library
add_library(CSR src/laplacian/CSR.cpp)

# Laplacian Gradient Library
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Testing programs
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE CSR Laplacian)

add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Main executable file
add_executable(firefly firefly.cpp)

# Enable testing environment
enable_testing()

# Add tests to the project
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)

# Add functionality for regression tests
include(add_regression_test)

# Prepare regression tests with expected pass/fail results.
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

# Doxygen Document Preparation
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_command(
        OUTPUT ${DOXYGEN_OUT}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

#Define the goal for generating documents
    add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})

# Add document dependency to main target
    add_dependencies(firefly doc)
endif()
