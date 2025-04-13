cmake_minimum_required(VERSION 3.10)
project(Firefly_Tests)

# Enable testing
enable_testing()

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find required packages
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Download and configure GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test subdirectory
add_subdirectory(user_input_tests)