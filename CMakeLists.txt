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

# Configure building the main executable

# Add source subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

#[===[ LUA ]===]
#[[ BEGIN ]]
# Add Lua as an external project dependency
ExternalProject_Add(lua
    URL http://www.lua.org/ftp/lua-5.4.4.tar.gz
    URL_HASH SHA256=164c7849653b80ae67bec4b7473b884bf5cc8d2dca05653475ec2ed27b9ebf61
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    CONFIGURE_COMMAND ""    # No configure step
    BUILD_IN_SOURCE 1
    BUILD_COMMAND ${CMAKE_COMMAND} -E echo "Building Lua for ${CMAKE_SYSTEM_NAME}"
    # More comprehensive platform detection
    COMMAND ${CMAKE_COMMAND} -E env
      ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/BuildLua.cmake
    INSTALL_COMMAND make INSTALL_TOP=${PROJECT_BINARY_DIR}/lua-install install
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/lua-install/include ${CMAKE_SOURCE_DIR}/src/user_input/LUA/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_BINARY_DIR}/lua-install/lib ${CMAKE_SOURCE_DIR}/src/user_input/LUA/lib
)

# Set Lua paths for other targets to use
set(LUA_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/user_input/LUA/include")
set(LUA_LIBRARIES "${CMAKE_SOURCE_DIR}/src/user_input/LUA/lib/liblua.a")

add_executable(user_input src/user_input/user_input.c)
add_dependencies(user_input lua)
target_include_directories(user_input PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(user_input PRIVATE ${LUA_LIBRARIES} m) # 'm' for the math library

#[[ END ]]
#[===] LUA [===]

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
target_link_libraries(LaplacianTests PRIVATE MatrixLib CSR Laplacian)

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
