# CMake project for Firefly
include(FetchContent)

cmake_minimum_required(VERSION 3.15)
project(Firefly VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ExternalProject)

# === Download fmt first (needed by exodus) ===
ExternalProject_Add(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.1.1
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/fmt-install
    -DFMT_DOC=OFF
    -DFMT_TEST=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
    -DFMT_INSTALL=ON
)

# === NetCDF (External) ===
ExternalProject_Add(netcdf
  GIT_REPOSITORY https://github.com/Unidata/netcdf-c.git
  GIT_TAG main
  CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/netcdf-install
    -DNETCDF_ENABLE_HDF5=OFF
    -DNETCDF_ENABLE_PARALLEL=OFF
    -DBUILD_SHARED_LIBS=ON
    -DNETCDF_ENABLE_TESTS=OFF
    -DNETCDF_ENABLE_EXAMPLES=OFF
    -DNETCDF_ENABLE_FSYNC=OFF
    -DENABLE_DAP=OFF
    -DENABLE_BYTERANGE=OFF
)

# === Exodus (SEACAS) ===
ExternalProject_Add(exodus
  GIT_REPOSITORY https://github.com/sandialabs/seacas.git
  GIT_TAG master
  DEPENDS fmt netcdf
  CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
    -DNetCDF_ROOT=${PROJECT_BINARY_DIR}/netcdf-install
    
    -Dfmt_DIR=${PROJECT_BINARY_DIR}/fmt-install/lib64/cmake/fmt

    -DSeacas_ENABLE_SEACAS=ON
    -DSeacas_ENABLE_SEACASExodus=ON
    -DSeacas_ENABLE_SEACASIoss=ON
    -DSeacas_ENABLE_SEACASNemesis=ON
)

# === Optional variables ===
set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")
set(NETCDF_LIB "${PROJECT_BINARY_DIR}/netcdf-install/lib/libnetcdf.so")
set(NETCDF_INCLUDE_DIR "${PROJECT_BINARY_DIR}/netcdf-install/include")

message("Install path: ${PROJECT_BINARY_DIR}")

# === Eigen ===
find_package(Eigen3 QUIET)
if (Eigen3_FOUND)
  message(STATUS "Using system-installed Eigen3: ${Eigen3_INCLUDE_DIRS}")
  include_directories(${Eigen3_INCLUDE_DIRS})
else()
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

# === Add user source files ===
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# === Libraries ===
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

add_library(CSR src/laplacian/CSR.cpp)
add_library(Laplacian src/laplacian/Laplacian.cpp)

# === Executables ===
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE MatrixLib CSR Laplacian)

add_executable(cholesky_test src/cholesky/cholesky_test.cpp)
target_link_libraries(cholesky_test PRIVATE MatrixLib)

add_executable(test_cholesky_CRS src/cholesky/test_cholesky_CRS.cpp)
target_link_libraries(test_cholesky_CRS PRIVATE MatrixLib)

add_executable(firefly firefly.cpp)

add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# === Test system ===
enable_testing()
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test)
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)
add_test(NAME cholesky_dense COMMAND cholesky_test)
add_test(NAME cholesky_crs COMMAND test_cholesky_CRS)

include(add_regression_test)
set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/test_runner.cmake)

add_regression_test(test_example_pass_regexp firefly
  EXTRA_PASS_REGEXP "Firefly Project"
  LABELS "basic")

add_regression_test(test_example_fail_regexp firefly
  EXTRA_FAIL_REGEXP "Firefly Project"
  EXTRA_PROPERTIES "WILL_FAIL;1"
  LABELS "basic")

# === Dependencies ===
add_dependencies(MatrixLib eigen)
add_dependencies(MatrixTests eigen)
add_dependencies(test_cholesky_CRS eigen)
add_dependencies(cholesky_test eigen)

