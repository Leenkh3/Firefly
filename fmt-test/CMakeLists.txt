cmake_minimum_required(VERSION 3.15)
project(FmtTest LANGUAGES CXX)

include(ExternalProject)

ExternalProject_Add(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 10.1.1
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/fmt-install
    -DFMT_DOC=OFF
    -DFMT_TEST=OFF
    -DBUILD_SHARED_LIBS=OFF
    -DFMT_INSTALL=ON
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON
)
