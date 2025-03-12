# Tell cmake the featureset we require
cmake_minimum_required(VERSION 3.15)
# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES CXX)
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

include(ExternalProject)

# Install NetCDF first
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
               -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/netcdf-install
)

# NetCDF Library Path
set(NETCDF_LIB "${PROJECT_BINARY_DIR}/netcdf-install/lib/libnetcdf.so")
set(NETCDF_INCLUDE_DIR "${PROJECT_BINARY_DIR}/netcdf-install/include")

# Install Exodus using the official Sandia Labs SEACAS repository
ExternalProject_Add(exodus
    GIT_REPOSITORY https://github.com/sandialabs/seacas.git
    GIT_TAG main
    DEPENDS netcdf  # Ensure NetCDF is built before Exodus
    CMAKE_ARGS -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DNetCDF_DIR=${PROJECT_BINARY_DIR}/netcdf-install
               -DBUILD_SHARED_LIBS=on
               -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
)

# Exodus Library Path
set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")
