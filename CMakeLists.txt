#Tell Cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES C CXX)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include ExternalProject module
include(ExternalProject)

# === ✅ ADD LOCAL EIGEN LIBRARY ===
# Path: firefly/eigen-3.4.0/eigen-3.4.0/Eigen
include_directories(${CMAKE_SOURCE_DIR}/eigen-3.4.0/eigen-3.4.0)

# Add source subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# adding matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
    src/cholesky/Cholesky_dense.cpp
    src/cholesky/cholesky.hpp
    src/cholesky/Cholesky_CRS.cpp
    src/cholesky/cholesky_CRS.hpp
)

# Configure a CSR matrix class
add_library(CSR src/laplacian/CSR.cpp)

# Configure a functions needed to compute the Laplacian
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Configure building the executable to test the Laplacian
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)

# Configure building the executable to test the Cholesky
add_executable(cholesky_test src/cholesky/cholesky_test.cpp)

add_executable(test_cholesky_CRS src/cholesky/test_cholesky_CRS.cpp)

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

# Add tests for the Cholesky
add_test(NAME cholesky COMMAND cholesky_test)

add_test(NAME cholesky COMMAND test_cholesky_CRS)

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

find_package(MPI)

# === KEEPING NetCDF ExternalProject ===
ExternalProject_Add(netcdf
    GIT_REPOSITORY https://github.com/Unidata/netcdf-c.git
    GIT_TAG main
    CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DENABLE_NETCDF_4=on
               -DBUILD_SHARED_LIBS=on
               -DENABLE_EXAMPLES=off
               -DENABLE_FSYNC=off
               -DENABLE_DAP=off
               -DENABLE_TESTS=off
               -DENABLE_CONVERSION_WARNINGS=off
               -DENABLE_DAP_REMOTE_TESTS=off
               -DENABLE_EXTREME_NUMBERS=off
               -DENABLE_PARALLEL4=on
               -DENABLE_PARALLEL_TESTS=off
               -DENABLE_BASH_SCRIPT_TESTING=off
               -DENABLE_V2_API=off
               -DNETCDF_ENABLE_HDF5=off
               -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/netcdf-install
)

# NetCDF Library Path
set(NETCDF_LIB "${PROJECT_BINARY_DIR}/netcdf-install/lib/libnetcdf.so")
set(NETCDF_INCLUDE_DIR "${PROJECT_BINARY_DIR}/netcdf-install/include")

# === 🔥 REMOVED ExternalProject_Add for Eigen — using local copy ===
# External project: Eigen (DISABLED in favor of local copy)
# ExternalProject_Add(eigen
#     GIT_REPOSITORY https://gitlab.com/eigen/eigen.git
#     GIT_TAG master
#     CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/eigen-install
# )
# set(EIGEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/eigen-install/include/eigen3")

# Add source subdirectory (your original setup)
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Install Exodus using the official Sandia Labs SEACAS repository
ExternalProject_Add(exodus
    GIT_REPOSITORY https://github.com/sandialabs/seacas.git
    GIT_TAG master
    DEPENDS netcdf  # Ensure NetCDF is built before Exodus
    CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DNetCDF_DIR=${PROJECT_BINARY_DIR}/netcdf-install
               -DBUILD_SHARED_LIBS=on
               -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
)

# Exodus Library Path
set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")

message("Install path: ${PROJECT_BINARY_DIR}")

