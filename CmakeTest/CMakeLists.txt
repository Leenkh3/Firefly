# Minimum CMake version
cmake_minimum_required(VERSION 3.10)
project(GmshReader)

# Add the main executable
add_executable(GmshReader gmsh_reader.c)

# Enable testing
enable_testing()

# Register a test with CTest
add_test(NAME RunGmshReader
         COMMAND GmshReader ${CMAKE_CURRENT_SOURCE_DIR}/test.msh ${CMAKE_CURRENT_BINARY_DIR}/output.txt)
