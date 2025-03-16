# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)
# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)
# Configure building the gmsh mesh reader
# add_library(GmshMeshReader GmshMeshReader.cpp GmshMeshReader.hpp)
# Configure building the main executable

# adding matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
)


add_executable(firefly firefly.cpp)

# Configure the link command
# Link the Matrix library to the main executable
target_link_libraries(firefly PRIVATE MatrixLib)

# target_link_libraries(firefly PRIVATE GmshMeshReader)
# Configure cmake's testing system
enable_testing()

add_executable(MatrixTests src/matrix/text_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)
# Add a test, running the chillax executable, non-zero exit code will fail the test
add_test(NAME firefly COMMAND firefly)

# Add a test for the Matrix library
add_test(NAME MatrixTests COMMAND MatrixTests)
