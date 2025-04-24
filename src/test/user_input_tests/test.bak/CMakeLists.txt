cmake_minimum_required(VERSION 3.10)
project(Firefly_Tests)

# Enable testing
enable_testing()

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Check required components
message(STATUS "Checking dependencies for Firefly tests...")

# Check for Lua
find_package(Lua)
if(NOT LUA_FOUND)
    message(WARNING "Lua not found - user input tests will be disabled")
    message(STATUS "Install Lua using your package manager:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install liblua5.3-dev")
    message(STATUS "  Fedora/RHEL:   sudo dnf install lua-devel")
    message(STATUS "  Arch Linux:    sudo pacman -S lua")
    message(STATUS "  macOS:         brew install lua")
    set(SKIP_USER_INPUT_TESTS TRUE)
else()
    message(STATUS "Found Lua: ${LUA_INCLUDE_DIR}")
    include_directories(${LUA_INCLUDE_DIR})
endif()

# Check for gfortran and other dependencies if needed
find_program(GFORTRAN gfortran)
if(NOT GFORTRAN)
    message(WARNING "Fortran compiler not found - some tests might fail")
    message(STATUS "Install gfortran using your package manager:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install gfortran")
    message(STATUS "  Fedora/RHEL:   sudo dnf install gcc-gfortran")
    message(STATUS "  Arch Linux:    sudo pacman -S gcc-fortran")
    message(STATUS "  macOS:         brew install gcc")
endif()

# Check for m4
find_program(M4 m4)
if(NOT M4)
    message(WARNING "m4 not found - some tests might fail")
    message(STATUS "Install m4 using your package manager:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install m4")
    message(STATUS "  Fedora/RHEL:   sudo dnf install m4")
    message(STATUS "  Arch Linux:    sudo pacman -S m4")
    message(STATUS "  macOS:         brew install m4")
endif()

# Find zlib
find_package(ZLIB)
if(NOT ZLIB_FOUND)
    message(WARNING "zlib not found - some tests might fail")
    message(STATUS "Install zlib using your package manager:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install zlib1g-dev")
    message(STATUS "  Fedora/RHEL:   sudo dnf install zlib-devel")
    message(STATUS "  Arch Linux:    sudo pacman -S zlib")
    message(STATUS "  macOS:         brew install zlib")
endif()

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
if(NOT SKIP_USER_INPUT_TESTS)
    add_subdirectory(user_input_tests)
else()
    message(STATUS "Skipping user input tests due to missing dependencies")
endif()