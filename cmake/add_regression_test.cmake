################################################################################
#
# \file      add_regression_test.cmake
# \brief     Function used to add a regression test to the ctest test suite
#
################################################################################

# ##############################################################################
# Function used to add a regression test to the ctest test suite
# add_regression_test( <test_name> <executable>
#                      [INPUTFILES file1 file2 ...]
#                      [ARGS arg1 arg2 ...]
#                      [LABELS label1 label2 ...]
#                      [EXTRA_PASS_REGEXP extra_pass_regexp]
#                      [EXTRA_FAIL_REGEXP extra_fail_regexp]
#                      [EXTRA_PROPERTIES extra_properties]
#
# Mandatory arguments:
# --------------------
#
# <test_name> - Name of the test.
# <executable> - Name of the executable to test.
#
# Optional arguments:
# -------------------
#
# INPUTFILES file1 file2 ... - Input files required for the test. This list of
# files includes files needed for running the test, e.g., control file
# (containing user input), mesh file, etc., as well as file required for
# evaluating the test, e.g., diff program configuration file. All input files
# are soft-linked from the source dir to the build dir. Default: "".
#
# ARGS arg1 arg2 ... - Arguments to pass to executable tested. Default: "".
#
# LABELS label1 label2 ... - Optional labels associated with the test.
# Default: "${executable}".
#
# EXTRA_PASS_REGEXP extra_pass_regexp - Extra pass regular expression
#
# EXTRA_FAIL_REGEXP extra_fail_regexp - Extra fail regular expression
#
# EXTRA_PROPERTIES extra_properties - Extra test properties to pass to add_test
#
# ##############################################################################
function(ADD_REGRESSION_TEST test_name executable)

  set(oneValueArgs EXTRA_PASS_REGEXP EXTRA_FAIL_REGEXP)
  set(multiValueArgs INPUTFILES ARGS EXTRA_PROPERTIES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}"
                        ${ARGN})

  # Will collect test properties
  set(test_properties)

  list(APPEND test_properties ${ARG_EXTRA_PROPERTIES})
  #message("prop: ${test_properties}")

  # Set and create test run directory
  set(workdir ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
  file(MAKE_DIRECTORY ${workdir})

  # Add labels to test
  if (ARG_LABELS)
    list(APPEND TEST_LABELS ${ARG_LABELS})
  endif()
  # prepare test labels to pass as cmake script arguments
  set(ARG_LABELS ${TEST_LABELS})
  string(REPLACE ";" " " ARG_LABELS "${ARG_LABELS}")

  # Construct and echo configuration for test being added
  set(msg "Add regression test ${test_name} for ${executable}")

  if (ARG_ARGS)
    string(REPLACE ";" " " ARGUMENTS "${ARG_ARGS}")
    string(CONCAT msg "${msg}, args: '${ARGUMENTS}'")
  endif()

  set(EXECUTABLE "${CMAKE_BINARY_DIR}/${executable}")

  # Add the test. See test_runner.cmake for documentation of the arguments.
  add_test(NAME ${test_name}
           COMMAND ${CMAKE_COMMAND}
           -DTEST_NAME=${test_name}
           -DWORKDIR=${workdir}
           -DTEST_EXECUTABLE=${EXECUTABLE}
           -DTEST_EXECUTABLE_ARGS=${ARGUMENTS}
           -DTEST_LABELS=${ARG_LABELS}
           -P ${TEST_RUNNER}
           WORKING_DIRECTORY ${workdir})

  # build pass regular expression list for test
  set(pass_regexp "")
  # add extra pass regexp
  list(APPEND pass_regexp "${ARG_EXTRA_PASS_REGEXP}")

  # build fail regular expression list for test
  set(fail_regexp "")
  # add extra fail regexp
  list(APPEND fail_regexp "${ARG_EXTRA_FAIL_REGEXP}")

  #message("'${test_name}' pass regexp: ${pass_regexp}, fail regexp: ${fail_regexp}")

  # Set test properties and instruct ctest to check textual diff output against
  # the regular expressions specified.
  set_tests_properties(${test_name} PROPERTIES ${test_properties}
                       PASS_REGULAR_EXPRESSION "${pass_regexp}"
                       FAIL_REGULAR_EXPRESSION "${fail_regexp}")

  # Set labels cmake test property. The LABELS built-in cmake property is not
  # passed as part of test_properties above in set_test_properties as
  # TEST_LABELS is a cmake list and passing in lists of lists does not work as
  # expected.
  set_property(TEST ${test_name} PROPERTY LABELS ${TEST_LABELS})

endfunction()
