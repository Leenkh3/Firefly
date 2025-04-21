#Tell Cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES C CXX)

# Set the compiler version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options for controlling tests and dependencies
option(FIREFLY_BUILD_TESTS "Build Firefly tests" ON)
option(FIREFLY_SKIP_EXODUS "Skip building EXODUS dependency" OFF)
option(FIREFLY_SKIP_NETCDF "Skip building NetCDF dependency" OFF)

# Check for critical dependencies if building tests
if(FIREFLY_BUILD_TESTS)
    # Check for Lua
    find_package(Lua QUIET)
    if(NOT LUA_FOUND)
        message(WARNING "Lua not found - user input tests will be disabled")
        message(STATUS "Install Lua or run scripts/install_dependencies.sh")
        set(FIREFLY_SKIP_USER_INPUT_TESTS TRUE)
    endif()
endif()

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

#[===[ LUA ]===]
#[[ BEGIN ]]
# First, try to find Lua on the system
find_package(Lua QUIET)

# Conditionally add Lua as external dependency only if not found
if(NOT LUA_FOUND)
  message(STATUS "System Lua not found. Building from source...")
  
  # Add Lua as an external project dependency
  ExternalProject_Add(lua
      URL http://www.lua.org/ftp/lua-5.4.4.tar.gz
      URL_HASH SHA256=164c7849653b80ae67bec4b7473b884bf5cc8d2dca05653475ec2ed27b9ebf61
      DOWNLOAD_EXTRACT_TIMESTAMP TRUE
      CONFIGURE_COMMAND ""    # No configure step
      BUILD_IN_SOURCE 1
      BUILD_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/BuildLua.cmake
      INSTALL_COMMAND make INSTALL_TOP=${PROJECT_BINARY_DIR}/lua-install install
  )
  
  # Set variables for built Lua
  set(LUA_INCLUDE_DIR "${PROJECT_BINARY_DIR}/lua-install/include")
  set(LUA_LIBRARIES "${PROJECT_BINARY_DIR}/lua-install/lib/liblua.a")
else()
  message(STATUS "Using system Lua: ${LUA_LIBRARIES}")
endif()

# Make Lua headers available for include
include_directories(${LUA_INCLUDE_DIR})

# Create an imported target for Lua
add_library(lua::lua STATIC IMPORTED)
set_target_properties(lua::lua PROPERTIES
    IMPORTED_LOCATION "${LUA_LIBRARIES}"
    INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIR}"
)

# Conditional dependencies on the ExternalProject target
if(NOT LUA_FOUND)
    add_dependencies(lua::lua lua)
endif()

# Now you can use the lua::lua target
add_library(user_input STATIC src/user_input/user_input.c)
target_include_directories(user_input PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(user_input PRIVATE lua::lua)

# Conditional dependency for user_input
if(NOT LUA_FOUND)
    add_dependencies(user_input lua)
endif()

target_include_directories(user_input PRIVATE ${LUA_INCLUDE_DIR}) # This is now redundant but harmless
target_link_libraries(user_input PRIVATE m) # 'm' for the math library

#[[ END ]]
#[===] LUA [===]

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

# Add the test directory conditionally
if(FIREFLY_BUILD_TESTS)
    add_subdirectory(src/test)
endif()

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

# === KEEPING NetCDF ExternalProject ===
ExternalProject_Add(netcdf
    GIT_REPOSITORY https://github.com/Unidata/netcdf-c.git
    GIT_TAG main
    CMAKE_ARGS
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
      -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/netcdf-install
      -DNETCDF_ENABLE_HDF5=off
      -DNETCDF_ENABLE_PARALLEL=off
      -DBUILD_SHARED_LIBS=on
      -DNETCDF_ENABLE_TESTS=off
      -DNETCDF_ENABLE_EXAMPLES=off
      -DNETCDF_ENABLE_FSYNC=off
      -DENABLE_DAP=OFF
      -DENABLE_BYTERANGE=OFF
)

# NetCDF Library Path
set(NETCDF_LIB "${PROJECT_BINARY_DIR}/netcdf-install/lib/libnetcdf.so")
set(NETCDF_INCLUDE_DIR "${PROJECT_BINARY_DIR}/netcdf-install/include")

# Install Exodus using the official Sandia Labs SEACAS repository

# fmt is required because SEACAS (Exodus) depends on it.
# We build it locally to ensure compatibility with NetCDF and SEAC

# === Add External fmt build ===
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

set(fmt_DIR "${PROJECT_BINARY_DIR}/fmt-install/lib/cmake/fmt")

ExternalProject_Add(exodus
    GIT_REPOSITORY https://github.com/sandialabs/seacas.git
    GIT_TAG master
    DEPENDS fmt netcdf
    CMAKE_ARGS
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}

        -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
        -DNetCDF_DIR=${PROJECT_BINARY_DIR}/netcdf-install
        -Dfmt_DIR=${PROJECT_BINARY_DIR}/fmt-install/lib/cmake/fmt
  #      -DTPL_ENABLE_fmt=ON
 #       -DTPL_fmt_INCLUDE_DIRS=${PROJECT_BINARY_DIR}/fmt-install/include
#        -DTPL_fmt_LIBRARIES=${PROJECT_BINARY_DIR}/fmt-install/lib64/libfmt.a
        -DSeacas_ENABLE_SEACAS=ON
        -DSeacas_ENABLE_SEACASExodus=ON
        -DSeacas_ENABLE_SEACASIoss=ON
        -DSeacas_ENABLE_SEACASNemesis=ON
)

# Exodus Library Path
set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")

if(NOT "${CMAKE_C_FLAGS}" MATCHES "std=c99")
   if(NOT CMAKE_C_COMPILER_ID STREQUAL "Cray")
     if(NOT (WIN32 AND CMAKE_C_COMPILER_ID STREQUAL "Intel") AND
        NOT CMAKE_C_COMPILER_ID STREQUAL "PGI")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
     else()
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Qstd=c99")
     endif()
   endif()
endif()

file(GLOB numdiff_src src/ndiff/src/*.c)

include_directories(src/ndiff/src)
add_executable(numdiff ${numdiff_src})
add_library(numdiff_lib STATIC ${numdiff_src})
add_library(numdiff-Executer STATIC src/ndiff/src/ndiff-executer.cpp)
set(OUTPUT_NAME maddiff CACHE STRING "Output name for executable")
set_target_properties(numdiff PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
if(UNIX)
   target_link_libraries(numdiff m)
endif()

install(TARGETS numdiff
   RUNTIME
   DESTINATION bin
   COMPONENT Runtime
  )

# add test executable for ndiff
add_executable(numdiff_test src/ndiff/test_ndiff.cpp)
target_link_libraries(numdiff_test PRIVATE m numdiff-Executer)

# Add a test, running the numdiff_test executable, non-zero exit code will fail the test
add_regression_test( numdifftest_pass numdiff_test
                     INPUTFILES src/ndiff/diag1.txt src/ndiff/diag3.txt src/ndiff/config.cfg
                     ARGS ${CMAKE_BINARY_DIR}/maddiff diag1.txt diag3.txt config.cfg)

# Add a test, running the numdiff_test executable, non-zero exit code will fail the test
add_regression_test( numdifftest_fail numdiff_test
                     INPUTFILES src/ndiff/diag1.txt src/ndiff/diag2.txt src/ndiff/config.cfg
                     ARGS ${CMAKE_BINARY_DIR}/maddiff diag1.txt diag2.txt config.cfg
                     EXTRA_FAIL_REGEXP "Files differ"
                     EXTRA_PROPERTIES "WILL_FAIL;1")

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
