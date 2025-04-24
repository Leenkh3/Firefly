cmake_minimum_required(VERSION 3.15)
project(Firefly VERSION 1.0 LANGUAGES C CXX)

# Set the compiler version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the path of the additional modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add source directory to main project
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Include ExternalProject module
include(ExternalProject)

# Use system Eigen if available, otherwise download it
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
            -DEIGEN_USE_LAPACKE=OFF
            -DEIGEN_BUILD_BLAS=OFF
            -DEIGEN_BUILD_LAPACK=OFF
    )
    set(EIGEN_INCLUDE_DIR "${PROJECT_BINARY_DIR}/eigen-install/include/eigen3")
    include_directories(${EIGEN_INCLUDE_DIR})
endif()

# Add libraries and executables
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
    src/CG/CGDense.cpp
    src/CG/CGMatrix.h
    src/CG/CGDense.h
    src/cholesky/Cholesky_dense.cpp
    src/cholesky/cholesky.hpp
    src/cholesky/Cholesky_CRS.cpp
    src/cholesky/cholesky_CRS.hpp
    src/cholesky/CholeskyCRSeigen.cpp
    src/laplacian/Laplacian.cpp
    src/cholesky/CholeskyCRSeigen.hpp
)

# Add testing
enable_testing()
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE MatrixLib)

add_executable(firefly firefly.cpp)

# Add other tests
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)

# Add regression tests
include(add_regression_test)
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
