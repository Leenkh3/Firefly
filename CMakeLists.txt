cmake_minimum_required(VERSION 3.15)
project(Firefly VERSION 1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
)

add_executable(ConjugateGradientSolver ConjugateGradientSolver.cpp)
target_link_libraries(ConjugateGradientSolver PRIVATE MatrixLib)

enable_testing()

add_executable(MatrixTests src/matrix/TestMatrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

add_test(NAME ConjugateGradientSolver COMMAND ConjugateGradientSolver)
add_test(NAME MatrixTests COMMAND MatrixTests)

include(AddConjugateGradientTest)

set(TEST_RUNNER ${CMAKE_SOURCE_DIR}/cmake/RunConjugateGradientTest.cmake)

add_conjugate_gradient_test(test_example_pass_regexp ConjugateGradientSolver
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_conjugate_gradient_test(test_example_fail_regexp ConjugateGradientSolver
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")
