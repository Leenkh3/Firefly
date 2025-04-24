add_executable(test_amgcl test_amgcl.cpp)
target_link_libraries(test_amgcl PUBLIC amgcl_solver Laplacian)
target_include_directories(test_amgcl PUBLIC ${CMAKE_SOURCE_DIR}/src/laplacian)
add_test(NAME amgcl_test COMMAND test_amgcl)