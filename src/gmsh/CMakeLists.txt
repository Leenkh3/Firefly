cmake_minimum_required(VERSION 3.10)
project(GmshReader)

# Add the main executable
add_executable(GmshReader src/gmsh/gmsh_reader.c)

# Enable testing
enable_testing()

# Define test input/output paths
set(TEST_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/test.msh)
set(TEST_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/output.txt)
set(EXPECTED_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/expected_output.txt)

# Register a test with CTest
add_test(NAME RunGmshReader
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/GmshReader ${TEST_INPUT} ${TEST_OUTPUT})

# Compare the output with the expected output
add_test(NAME CompareOutput
         COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_OUTPUT} ${EXPECTED_OUTPUT})
