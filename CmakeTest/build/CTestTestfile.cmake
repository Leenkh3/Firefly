# CMake generated Testfile for 
# Source directory: /work/students/hpc-f0v113/firefly/Firefly/CmakeTest
# Build directory: /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(RunGmshReader "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/GmshReader" "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/test.msh" "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/output.txt")
set_tests_properties(RunGmshReader PROPERTIES  _BACKTRACE_TRIPLES "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/CMakeLists.txt;16;add_test;/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/CMakeLists.txt;0;")
add_test(CompareOutput "/work/students/hpc-f0v113/cmake/bin/cmake" "-E" "compare_files" "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/output.txt" "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/expected_output.txt")
set_tests_properties(CompareOutput PROPERTIES  _BACKTRACE_TRIPLES "/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/CMakeLists.txt;20;add_test;/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/CMakeLists.txt;0;")
