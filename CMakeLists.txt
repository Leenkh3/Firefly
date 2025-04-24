 saher
# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Project name, version, and languages ​​used
project(Firefly VERSION 1.0 LANGUAGES CXX)


#Tell Cmake the featureset we require
cmake_minimum_required(VERSION 3.15)

# Project name and version with languages/compilers required
project(Firefly VERSION 1.0 LANGUAGES C CXX)

# Set the compiler version to 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 main

# Set the path of the additional modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

 saher
# Add source directory to main project
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

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
 main

# Library for dealing with arrays
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
    src/cholesky/CholeskyCRSeigen.cpp
    src/laplacian/Laplacian.cpp

    src/cholesky/CholeskyCRSeigen.hpp

)

#CSR Library
add_library(CSR src/laplacian/CSR.cpp)

 saher
# Laplacian Gradient Library

# Configure a functions needed to compute the Laplacian
 main
add_library(Laplacian src/laplacian/Laplacian.cpp)

# Testing programs
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE MatrixLib CSR Laplacian)

# Configure building the executable to test the Cholesky
add_executable(cholesky_test src/cholesky/cholesky_test.cpp)
target_link_libraries(cholesky_test PRIVATE MatrixLib)

add_executable(test_cholesky_CRS src/cholesky/test_cholesky_CRS.cpp)
target_link_libraries(test_cholesky_CRS PRIVATE MatrixLib)

add_executable(testCholeskyCRSeigen src/cholesky/testCholeskyCRSeigen.cpp)

target_link_libraries(testCholeskyCRSeigen PRIVATE MatrixLib)


 saher
add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# Main executable file
add_executable(firefly firefly.cpp)


# Configure building the CG solver executable
add_executable(ConjugateGradientSolver src/CG/ConjugateGradientSolver.cpp)
target_link_libraries(ConjugateGradientSolver PRIVATE MatrixLib)

# Add source subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

add_executable(firefly firefly.cpp)  # Configure cmake's testing system
 main

# Enable testing environment
enable_testing()

 saher
# Add tests to the project

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

add_dependencies(testCholeskyCRSeigen eigen)
# Add a test, running the chillax executable, non-zero exit code will fail the test
 main
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
 saher
add_test(NAME laplacian COMMAND LaplacianTests)

# Add functionality for regression tests
=======

# Add tests for the Laplacian
add_test(NAME laplacian COMMAND LaplacianTests)

# Add tests for CG solver
add_test(NAME ConjugateGradientSolver COMMAND ConjugateGradientSolver)
add_test(NAME CGMatrixTests COMMAND CGMatrixTests)

# Add tests for the Cholesky
add_test(NAME cholesky_dense COMMAND cholesky_test)
add_test(NAME cholesky_crs COMMAND test_cholesky_CRS)
add_test(NAME testCholeskyCRSeigen COMMAND testCholeskyCRSeigen)

# Define the source and destination paths for the Resources directory symlink
set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/Resources")
set(SYMLINK_DIR "${CMAKE_BINARY_DIR}/Resources")

# Check if the symlink or directory already exists
if(NOT EXISTS "${SYMLINK_DIR}")
  # Create a symbolic link in the build folder to the Resources directory in the src folder
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${RESOURCES_DIR}" "${SYMLINK_DIR}"
    RESULT_VARIABLE ügy
  )
  if(result)
    message(WARNING "Failed to create symlink to resources directory: ${result}")
  else()
    message(STATUS "Created symlink to Resources at: ${SYMLINK_DIR}")
  endif()
endif()

# Add ASC reader file
add_library(asc src/asc/asc.cpp)
add_executable(AscTest src/asc/asc_test.cpp)
target_link_libraries(AscTest PRIVATE asc)
add_test(NAME AscTest COMMAND AscTest)

# Include function used to add regression tests
 main
include(add_regression_test)

# Prepare regression tests with expected pass/fail results.
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

 saher
# Doxygen Document Preparation
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_command(
        OUTPUT ${DOXYGEN_OUT}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

#Define the goal for generating documents
    add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})

# Add document dependency to main target
    add_dependencies(firefly doc)
endif()

# Add regression tests for CG solver
add_regression_test(test_cg_pass_regexp ConjugateGradientSolver
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_cg_fail_regexp ConjugateGradientSolver
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

find_package(MPI)

set(CMAKE_PREFIX_PATH "/usr/local")
find_package(NetCDF)
if(NOT NetCDF_FOUND)
    message(STATUS "NetCDF NOT found. Downloading with ExternalProject_Add().")
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
    set(NETCDF_DIR "${PROJECT_BINARY_DIR}/netcdf-install")
else()
    message(STATUS "NetCDF found.")
    set(netcdf "")
    #set(NETCDF_LIB "${CMAKE_PREFIX_PATH}/lib/libnetcdf.so")
    #set(NETCDF_INCLUDE_DIRS "${CMAKE_PREFIX_PATH}/include")
    #set(NETCDF_DIR "${CMAKE_PREFIX_PATH}")
endif()


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

find_package(Exodus)
if(NOT Exodus_FOUND)
    message(STATUS "Exodus NOT found. Downloading with ExternalProject_Add().")
    # Install Exodus using the official Sandia Labs SEACAS repository
    ExternalProject_Add(exodus
    GIT_REPOSITORY https://github.com/sandialabs/seacas.git
    GIT_TAG master
    DEPENDS fmt ${netcdf}
    CMAKE_ARGS
        -DCMAKE_C_COMPILER=${MPI_C_COMPILER}
        -DCMAKE_CXX_COMPILER=${MPI_CXX_COMPILER}

        -DCMAKE_INSTALL_PREFIX=${PROJECT_BINARY_DIR}/exodus-install
        #-DNetCDF_DIR=${PROJECT_BINARY_DIR}/netcdf-install
        -DNetCDF_ROOT=${NETCDF_DIR}
        -DNetCDF_LIBRARIES=${NETCDF_LIBRARIES}
        -DNetCDF_INCLUDE_DIRS=${NETCDF_INCLUDE_DIRS}
        -Dfmt_DIR=${PROJECT_BINARY_DIR}/fmt-install/lib/cmake/fmt
#        -DTPL_ENABLE_fmt=ON
#        -DTPL_fmt_INCLUDE_DIRS=${PROJECT_BINARY_DIR}/fmt-install/include
#        -DTPL_fmt_LIBRARIES=${PROJECT_BINARY_DIR}/fmt-install/lib64/libfmt.a
        -DSeacas_ENABLE_SEACAS=ON
        -DSeacas_ENABLE_SEACASExodus=ON
        -DSeacas_ENABLE_SEACASIoss=OFF
        #-DSeacas_ENABLE_SEACASNemesis=ON
    )
    # Exodus Library Path
    set(EXODUS_LIB "${PROJECT_BINARY_DIR}/exodus-install/lib/libexodus.so")
else()
    message(STATUS "Seacas found.")
    set(EXODUS_LIB "${CMAKE_PREFIX_PATH}/lib/libexodus.so")
endif()


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
# Configure the link command
# target_link_libraries(firefly PRIVATE GmshMeshReader)
# Configure cmake's testing system
#enable_testing()
# Add a test, running the chillax executable, non-zero exit code will fail the test
#add_test(NAME firefly COMMAND firefly)

message("Install path: ${PROJECT_BINARY_DIR}")
 main
