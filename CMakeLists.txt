#Tell Cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES C CXX)

# Set the compiler version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set cmake modules directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Include ExternalProject module
include(ExternalProject)

# Add source subdirectory (adjusted for CG)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/CG)

# adding matrix to the build
add_library(MatrixLib
    src/CG/CGDense.cpp
    src/CG/CGMatrix.h
    src/CG/CGDense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
)

# Configure a CSR matrix class
add_library(CSR src/laplacian/CSR.cpp)
# Configure a functions needed to compute the Laplacian
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Configure building the executable to test the Laplacian
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE MatrixLib CSR Laplacian)

# Configure building the CG solver executable
add_executable(ConjugateGradientSolver src/CG/ConjugateGradientSolver.cpp)
target_link_libraries(ConjugateGradientSolver PRIVATE MatrixLib)

# Configure cmake's testing system
enable_testing()

# Add testing executable for CG matrix
add_executable(CGMatrixTests src/CG/CGTestMatrix.cpp)
target_link_libraries(CGMatrixTests PRIVATE MatrixLib)

# Add tests
add_test(NAME ConjugateGradientSolver COMMAND ConjugateGradientSolver)
add_test(NAME CGMatrixTests COMMAND CGMatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)

# Include function used to add regression tests
include(add_regression_test)

# Set paths required for running tests
set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/test_runner.cmake)

# Add regression tests for CG solver
add_regression_test(test_cg_pass_regexp ConjugateGradientSolver
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_cg_fail_regexp ConjugateGradientSolver
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

find_package(MPI)

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

ExternalProject_Add(exodus
    GIT_REPOSITORY https://github.com/sandialabs/seacas.git
    GIT_TAG master
    DEPENDS netcdf
    CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DNetCDF_DIR=${PROJECT_BINARY_DIR}/netcdf-install
               -DBUILD_SHARED_LIBS=on
               -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
)

# Exodus Library Path
set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")

message("Install path: ${PROJECT_BINARY_DIR}")
