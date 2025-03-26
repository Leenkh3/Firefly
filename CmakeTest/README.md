# Basic instructions for building, compiling and testing the gmsh reader code
## How to run ctest:
cd build

cmake ..

cmake --build .

ctest --output-on-failure

## Compile and run the gmsh reader:
gcc -o gmsh gmsh_reader.c

./gmsh test.msh output.txt

This way, by using command line arguments the code reads the test.msh file and writes the node coordinates and the element connectivity into a txt file. The code checks if the output is identical with the initial values.


