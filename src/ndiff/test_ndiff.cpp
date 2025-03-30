
#include "ndiff-executer.hpp"
#include <iostream>

int test_ndiff(){
    NdiffExecuter ndiffExecuter("file1", "file2", "config");
    std::string output = ndiffExecuter.execute();

    // Assuming the expected output is known
    // For example, let's say the expected output is:
    std::string expected_output = "processing 'diag1.txt'|'diag3.txt'/n  5584 lines have been diffed";


    // Check if the output matches the expected output
    if (output != expected_output){
        std::cout << "Test failed" << std::endl;
        return 1;
    }
    return 0;
}

int main(){
    NdiffExecuter ndiffExecuter("file1", "file2", "config");
    ndiffExecuter.execute();
    return 0;
}
