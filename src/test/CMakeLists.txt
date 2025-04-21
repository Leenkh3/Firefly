# Check for required components
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

# Download and configure GoogleTest if needed
if(NOT TARGET gtest)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG release-1.12.1
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# Add user_input_tests subdirectory if dependencies are available
if(NOT SKIP_USER_INPUT_TESTS)
    add_subdirectory(user_input_tests)
else()
    message(STATUS "Skipping user input tests due to missing dependencies")
endif()

add_executable(test_amgcl test_amgcl.cpp)
target_link_libraries(test_amgcl PUBLIC amgcl_solver)
add_test(NAME amgcl_test COMMAND test_amgcl)