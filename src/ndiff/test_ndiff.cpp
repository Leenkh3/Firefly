#include "ndiff-executer.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

// Helper function to create a temporary file with content
void createTempFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to create temporary file: " + filename);
    }
    file << content;
    file.close();
}

// Test function for NdiffExecuter
int test_ndiff(char * ndiffexcutable, char * file1, char * file2, char * config) {
    try {
        // Create an instance of NdiffExecuter
        NdiffExecuter ndiffExecuter(file1, file2, config, ndiffexcutable);

        // Execute the ndiff tool
        std::string output = ndiffExecuter.execute();

        // Define the expected output based on actual ndiff behavior with the test files
        // Note: The expected output may vary based on the actual content of the files
        std::string expected_output = "processing 'diag1.txt'|'diag3.txt'/n  16 lines have been diffed"; // Example expected output

        // Validate the output
        if (output != expected_output) {
            std::cout << "Test failed: Output does not match expected output." << std::endl;
            std::cout << "Expected: " << expected_output << std::endl;
            std::cout << "Actual: " << output << std::endl;
            return 1;
        }

        // If we reach here, the test passed
        std::cout << "Test passed!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}

int main(int argc, char ** argv) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <ndiff_executable> <file1> <file2> <config>" << std::endl;
        return 1;
    }
    return test_ndiff(argv[1], argv[2], argv[3], argv[4]);
}