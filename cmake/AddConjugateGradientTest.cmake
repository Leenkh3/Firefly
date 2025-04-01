function(ADD_CONJUGATE_GRADIENT_TEST test_name executable)
  set(oneValueArgs EXTRA_PASS_REGEXP EXTRA_FAIL_REGEXP)
  set(multiValueArgs INPUTFILES ARGS EXTRA_PROPERTIES)
  cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(test_properties)
  list(APPEND test_properties ${ARG_EXTRA_PROPERTIES})

  set(workdir ${CMAKE_CURRENT_BINARY_DIR}/${test_name})
  file(MAKE_DIRECTORY ${workdir})

  if (ARG_LABELS)
    list(APPEND TEST_LABELS ${ARG_LABELS})
  endif()
  set(ARG_LABELS ${TEST_LABELS})
  string(REPLACE ";" " " ARG_LABELS "${ARG_LABELS}")

  set(msg "Add conjugate gradient test ${test_name} for ${executable}")
  if (ARG_ARGS)
    string(REPLACE ";" " " ARGUMENTS "${ARG_ARGS}")
    string(CONCAT msg "${msg}, args: '${ARGUMENTS}'")
  endif()

  set(EXECUTABLE "${CMAKE_BINARY_DIR}/${executable}")

  add_test(NAME ${test_name}
           COMMAND ${CMAKE_COMMAND}
           -DTEST_NAME=${test_name}
           -DWORKDIR=${workdir}
           -DTEST_EXECUTABLE=${EXECUTABLE}
           -DTEST_EXECUTABLE_ARGS=${ARGUMENTS}
           -DTEST_LABELS=${ARG_LABELS}
           -P ${TEST_RUNNER}
           WORKING_DIRECTORY ${workdir})

  set(pass_regexp "")
  list(APPEND pass_regexp "${ARG_EXTRA_PASS_REGEXP}")

  set(fail_regexp "")
  list(APPEND fail_regexp "${ARG_EXTRA_FAIL_REGEXP}")

  set_tests_properties(${test_name} PROPERTIES ${test_properties}
                       PASS_REGULAR_EXPRESSION "${pass_regexp}"
                       FAIL_REGULAR_EXPRESSION "${fail_regexp}")

  set_property(TEST ${test_name} PROPERTY LABELS ${TEST_LABELS})
endfunction()
