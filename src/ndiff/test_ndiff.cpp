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

bool outputHasDifferences(const std::string& output) {
    // Adjust the logic based on your ndiff output specifics
    return output.find("warng:") != std::string::npos;
}



// Test function for NdiffExecuter
int test_ndiff(char * ndiffexcutable, char * file1, char * file2, char * config) {
    try {
        // Create an instance of NdiffExecuter
        NdiffExecuter ndiffExecuter(file1, file2, config, ndiffexcutable);

        // Execute the ndiff tool
        std::string output = ndiffExecuter.execute();


        if (outputHasDifferences(output)) {
            std::cerr << "Error: Files differ!" << std::endl;
            // Optionally log or process further details here
            return 1;
        } else {
            std::cout << "Test passed: Files are strictly equal." << std::endl;
            return 0;
        }


        // Define the expected output based on actual ndiff behavior with the test files
        // Note: The expected output may vary based on the actual content of the files

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