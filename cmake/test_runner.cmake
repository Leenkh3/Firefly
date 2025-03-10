################################################################################
#
# \file      test_runner.cmake
# \brief     Regression test runner using the cmake scripting language
#
################################################################################

# Covert string to list of test labels
string(REPLACE " " ";" TEST_LABELS "${TEST_LABELS}")
# Covert string to list of test executable arguments
string(REPLACE " " ";" TEST_EXECUTABLE_ARGS "${TEST_EXECUTABLE_ARGS}")

# Print test runner configuration
message("Test runner configuration:")
message("  TEST_NAME (name of test)                                    : ${TEST_NAME}")
message("  WORKDIR (test run directory)                                : ${WORKDIR}")
message("  TEST_EXECUTABLE (executable tested)                         : ${TEST_EXECUTABLE}")
message("  TEST_EXECUTABLE_ARGS (executable arguments)                 : ${TEST_EXECUTABLE_ARGS}")
message("  TEST_LABELS (test labels)                                   : ${TEST_LABELS}")

set(test_command ${TEST_EXECUTABLE} ${TEST_EXECUTABLE_ARGS})


string(REPLACE ";" " " test_command_string "${test_command}")

# Run the test
message("\nRunning test command: '${test_command_string}'\n")
execute_process(COMMAND ${test_command} RESULT_VARIABLE ERROR)

# Check return value from test
if(ERROR)

  message(FATAL_ERROR "Test failed to run: '${test_command_string}' returned error code: ${ERROR}")

else() # Test command ran successfully, do something extra after test run

  # ...

endif()
