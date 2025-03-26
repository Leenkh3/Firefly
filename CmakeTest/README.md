# Basic instructions for building, compiling and testing the gmsh reader code
## How to run ctest:
cd build

cmake ..

cmake --build .

ctest --output-on-failure

## Compile and run the gmsh reader:
gcc -o gmsh gmsh_reader.c
./gmsh test.msh output.txt



