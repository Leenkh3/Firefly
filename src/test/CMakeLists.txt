add_executable(test_amgcl test_amgcl.cpp)
target_link_libraries(test_amgcl PUBLIC amgcl_solver)
add_test(NAME amgcl_test COMMAND test_amgcl)