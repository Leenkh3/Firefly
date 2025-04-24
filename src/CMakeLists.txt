# Add AMGCL library
add_library(amgcl_solver ${CMAKE_SOURCE_DIR}/src/amgcl_solver.cpp)
target_include_directories(amgcl_solver PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_include_directories(amgcl_solver PUBLIC ${CMAKE_SOURCE_DIR}/src/matrix)
target_link_libraries(amgcl_solver PUBLIC MatrixLib)