# User Input Tests

# Source files for the tests
set(TEST_SOURCES
    user_input_test.cpp
)

# Source files from the main project needed for testing
set(PROJECT_SOURCES
    ${CMAKE_SOURCE_DIR}/src/user_input/user_input.c
)

# Create test executable
add_executable(user_input_tests ${TEST_SOURCES} ${PROJECT_SOURCES})

# Add the proper include directory
target_include_directories(user_input_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link dependencies
target_link_libraries(user_input_tests
    gtest
    gtest_main
    ${LUA_LIBRARIES}
)

# Copy the test Lua script to the build directory
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/test_input_handler.lua
    ${CMAKE_CURRENT_BINARY_DIR}/test_input_handler.lua
    COPYONLY
)

# Add tests to be run
add_test(NAME UserInputTests COMMAND user_input_tests)

# Set environment variables for tests if needed
set_tests_properties(UserInputTests PROPERTIES
    ENVIRONMENT "FIREFLY_SCRIPT_PATH=${CMAKE_CURRENT_BINARY_DIR}/test_input_handler.lua"
)