
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

# ===  Use system Eigen if available, otherwise download it ===
find_package(Eigen3 QUIET)

if (Eigen3_FOUND)
    message(STATUS "Using system-installed Eigen3: ${Eigen3_INCLUDE_DIRS}")
    include_directories(${Eigen3_INCLUDE_DIRS})
else()
    message(STATUS "Eigen3 not found. Downloading with ExternalProject_Add.")

    include(ExternalProject)
    ExternalProject_Add(eigen
        GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
        GIT_TAG master
        CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/eigen-install
                -DEIGEN_BUILD_DOC=OFF
                -DEIGEN_BUILD_TESTING=OFF
                -DEIGEN_USE_LAPACKE=OFF         #  Disable LAPACK bindings
                -DEIGEN_BUILD_BLAS=OFF          #  Disable internal BLAS
                -DEIGEN_BUILD_LAPACK=OFF        #  Disable internal LAPACK
    )

    # Make sure to include the downloaded Eigen headers
    set(EIGEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/eigen-install/include/eigen3")
    include_directories(${EIGEN_INCLUDE_DIR})  
endif()

# Add source subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# adding matrix to the build
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
    src/CG/CGDense.cpp      # CG dense matrix operations (from HEAD)
    src/CG/CGMatrix.h       # CG matrix function declarations (from HEAD)
    src/CG/CGDense.h        # CG dense matrix header (from HEAD)
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
target_link_libraries(LaplacianTests PRIVATE MatrixLib CSR Laplacian)

# Configure building the executable to test the Cholesky
add_executable(cholesky_test src/cholesky/cholesky_test.cpp)
target_link_libraries(cholesky_test PRIVATE MatrixLib)

add_executable(test_cholesky_CRS src/cholesky/test_cholesky_CRS.cpp)
target_link_libraries(test_cholesky_CRS PRIVATE MatrixLib)

# Configure building the CG solver executable
add_executable(ConjugateGradientSolver src/CG/ConjugateGradientSolver.cpp)
target_link_libraries(ConjugateGradientSolver PRIVATE MatrixLib)

add_executable(firefly firefly.cpp)  # Configure cmake's testing system

# Configure cmake's testing system
enable_testing()

# Add testing subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test)

add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Add testing executable for CG matrix
add_executable(CGMatrixTests src/CG/CGTestMatrix.cpp)

# Link the Matrix library to the main executable
target_link_libraries(CGMatrixTests PRIVATE MatrixLib)

add_dependencies(MatrixLib eigen)
add_dependencies(MatrixTests eigen)
add_dependencies(test_cholesky_CRS eigen)
add_dependencies(cholesky_test eigen)

# Add a test, running the chillax executable, non-zero exit code will fail the test
add_test(NAME firefly COMMAND firefly)

# Add a test for the Matrix library
add_test(NAME MatrixTests COMMAND MatrixTests)

# Add tests for the Laplacian
add_test(NAME laplacian COMMAND LaplacianTests)

# Add tests for CG solver
add_test(NAME ConjugateGradientSolver COMMAND ConjugateGradientSolver)
add_test(NAME CGMatrixTests COMMAND CGMatrixTests)

# Add tests for the Cholesky
add_test(NAME cholesky_dense COMMAND cholesky_test)
add_test(NAME cholesky_crs COMMAND test_cholesky_CRS)

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
else()
    message(STATUS "NetCDF found.")
    set(NETCDF_LIB "${CMAKE_PREFIX_PATH}/netcdf-install/lib/libnetcdf.so")
    set(NETCDF_INCLUDE_DIRS "${CMAKE_PREFIX_PATH}/netcdf-install/include")
endif()

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

# Configure building the gmsh mesh reader
# add_library(GmshMeshReader GmshMeshReader.cpp GmshMeshReader.hpp)

# Configure building the main executable
#add_executable(firefly firefly.cpp)

# Configure the link command
# target_link_libraries(firefly PRIVATE GmshMeshReader)
# Configure cmake's testing system
#enable_testing()
# Add a test, running the chillax executable, non-zero exit code will fail the test
#add_test(NAME firefly COMMAND firefly)

message("Install path: ${PROJECT_BINARY_DIR}")
