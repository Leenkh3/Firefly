string(REPLACE " " ";" TEST_LABELS "${TEST_LABELS}")
string(REPLACE " " ";" TEST_EXECUTABLE_ARGS "${TEST_EXECUTABLE_ARGS}")

message("Test runner configuration:")
message("  TEST_NAME (name of test)                                    : ${TEST_NAME}")
message("  WORKDIR (test run directory)                                : ${WORKDIR}")
message("  TEST_EXECUTABLE (executable tested)                         : ${TEST_EXECUTABLE}")
message("  TEST_EXECUTABLE_ARGS (executable arguments)                 : ${TEST_EXECUTABLE_ARGS}")
message("  TEST_LABELS (test labels)                                   : ${TEST_LABELS}")

set(test_command ${TEST_EXECUTABLE} ${TEST_EXECUTABLE_ARGS})

string(REPLACE ";" " " test_command_string "${test_command}")

message("\nRunning test command: '${test_command_string}'\n")
execute_process(COMMAND ${test_command} RESULT_VARIABLE ERROR)

if(ERROR)
  message(FATAL_ERROR "Test failed to run: '${test_command_string}' returned error code: ${ERROR}")
else()
endif()
