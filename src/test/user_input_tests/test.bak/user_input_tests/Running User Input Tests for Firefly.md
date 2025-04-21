# Running User Input Tests for Firefly

This guide explains how to run the Google Test-based tests for the user input module in the Firefly project.

## Using CMake and Command Line

The simplest approach is to build and run the tests from the command line:

```bash
# Navigate to the project root
cd /home/martin/Firefly

# Create a build directory if it doesn't exist
mkdir -p build

# Navigate to the build directory
cd build

# Configure the project with CMake
cmake ..

# Build all targets including tests
make

# Run all tests using CTest
ctest

# To run with more detailed output:
ctest -V

# To run only the user input tests:
ctest -R UserInputTests
```

## Running Specific Test Executables Directly

You can also run the test executable directly for more control:

```bash
# From the build directory
cd /home/martin/Firefly/build
./test/user_input_tests/user_input_tests

# To run specific test cases:
./test/user_input_tests/user_input_tests --gtest_filter=UserInputTest.ProcessHelp
```

## Using VS Code Integration

Your VS Code should have test discovery for Google Test. You can:

1. Open the Command Palette (Ctrl+Shift+P)
2. Type and select "View: Show Testing" to open the test explorer
3. Click on the "Run All Tests" icon or run specific tests

## Test Environment Setup

The tests are configured to use test Lua scripts created specifically for testing:

- `test_input_handler.lua` for basic tests
- `advanced_test_script.lua` for advanced tests

The test environment is set up in the test fixture classes' `SetUp()` methods, and test scripts are created programmatically during the test execution.

## Troubleshooting

If you encounter issues with Lua not being found:

```bash
# Make sure you have Lua development libraries installed
sudo apt-get install liblua5.3-dev

# Or rebuild with explicit Lua path if needed
cmake -DLUA_INCLUDE_DIR=/path/to/lua/include -DLUA_LIBRARIES=/path/to/lua/lib/liblua.so ..
```

The tests validate various aspects of the user input system:
- Command processing (help, load, export, exit)
- Interactive mode
- Error handling
- Script loading