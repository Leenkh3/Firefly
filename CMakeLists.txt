# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)
# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)
# Configure building the gmsh mesh reader
# add_library(GmshMeshReader GmshMeshReader.cpp GmshMeshReader.hpp)
# Configure building the main executable
add_executable(firefly firefly.cpp)
# Configure the link command
# target_link_libraries(firefly PRIVATE GmshMeshReader)
# Configure cmake's testing system
enable_testing()
# Add a test, running the chillax executable, non-zero exit code will fail the test
add_test(NAME firefly COMMAND firefly)
