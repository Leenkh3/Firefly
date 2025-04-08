# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add source subdirectory for the main code
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Libraries to handle matrix operations
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
)

# Configure CSR matrix class
add_library(CSR src/laplacian/CSR.cpp)

# Configure Laplacian computation functions
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Test executables
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE CSR Laplacian)

# Main executable
add_executable(firefly firefly.cpp)

# Enable testing framework
enable_testing()

# Add testing subdirectory for additional tests
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test)

# Test executable for Matrix library
add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Register tests with CTest
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)

# Include function for adding regression tests
include(add_regression_test)

# Set paths required for running tests
set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/test_runner.cmake)

# Add regression tests with specific pass/fail expectations
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

# Doxygen documentation configuration
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


    add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})

 
    add_dependencies(all doc)
endif()
