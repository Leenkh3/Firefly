# Basic instructions for building, compiling and testing the gmsh reader code

cd build
cmake ..
cmake --build .
ctest --output-on-failure

