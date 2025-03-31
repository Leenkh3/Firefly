# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add source subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# adding matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
)

# Configure a CSR matrix class
add_library(CSR src/laplacian/CSR.cpp)
# Configure a functions needed to compute the Laplacian
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Configure building the executable to test the Laplacian
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE CSR Laplacian)

add_executable(firefly firefly.cpp)

# Configure cmake's testing system
enable_testing()

# Add testing subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test)

add_executable(MatrixTests src/matrix/test_matrix.cpp)
# Link the Matrix library to the main executable
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Add a test, running the chillax executable, non-zero exit code will fail the test
add_test(NAME firefly COMMAND firefly)

# Add a test for the Matrix library
add_test(NAME MatrixTests COMMAND MatrixTests)
# Add tests for the Laplacian
add_test(NAME laplacian COMMAND LaplacianTests)

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
# find package
find_package(Doxygen REQUIRED)
# add_custom_command
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
endif()

set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)

add_custom_target(
    generate_docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating documentation..."
    VERBATIM
)

add_dependencies(all generate_docs)

