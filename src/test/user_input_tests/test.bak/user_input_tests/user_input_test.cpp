#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "user_input/user_input.h"
}

class UserInputTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test script files
        createTestScriptFile();
    }

    void TearDown() override {
        // Clean up test files
        std::remove("./test_input_handler.lua");
    }

    void createTestScriptFile() {
        std::ofstream scriptFile("./test_input_handler.lua");
        scriptFile << R"(
-- Test input handler script for unit tests

-- Function to process command line arguments
function process_arguments(args)
    -- For testing, we'll return predefined values based on the first argument
    local operation = "load"
    local filetype = "csv"
    local filename = "test.csv"
    
    if args[2] == "help" then
        operation = "help"
        filetype = ""
        filename = ""
        print("Help: This is a test help message")
    elseif args[2] == "export" then
        operation = "export"
        filetype = "json"
        filename = "output.json"
    elseif args[2] == "exit" then
        operation = "exit"
        filetype = ""
        filename = ""
    end
    
    return operation, filetype, filename
end

-- Function for interactive user input
function get_user_input()
    -- For testing, we'll just return fixed values
    local operation = "load"
    local filetype = "csv"
    local filename = "input.csv"
    
    print("Interactive mode called")
    
    return operation, filetype, filename
end
)";
        scriptFile.close();
    }
};

// Test case: Process with help argument
TEST_F(UserInputTest, ProcessHelp) {
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments
    const char* args[] = {"program", "help"};
    
    // Call the function
    int result = processInput(2, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(output.find("Operation: help") != std::string::npos);
}

// Test case: Process with export argument
TEST_F(UserInputTest, ProcessExport) {
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments
    const char* args[] = {"program", "export"};
    
    // Call the function
    int result = processInput(2, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(output.find("Operation: export") != std::string::npos);
    EXPECT_TRUE(output.find("Filetype: json") != std::string::npos);
    EXPECT_TRUE(output.find("Filename: output.json") != std::string::npos);
}

// Test case: Process with default load (no arguments)
TEST_F(UserInputTest, ProcessNoArguments) {
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments (just program name)
    const char* args[] = {"program"};
    
    // Call the function
    int result = processInput(1, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(output.find("No arguments provided, starting interactive mode") != std::string::npos);
    EXPECT_TRUE(output.find("Operation: load") != std::string::npos);
    EXPECT_TRUE(output.find("Filetype: csv") != std::string::npos);
    EXPECT_TRUE(output.find("Filename: input.csv") != std::string::npos);
}

// Test case: Test exit command
TEST_F(UserInputTest, ProcessExit) {
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments
    const char* args[] = {"program", "exit"};
    
    // Call the function
    int result = processInput(2, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(output.find("Operation: exit") != std::string::npos);
    EXPECT_TRUE(output.find("Exiting program") != std::string::npos);
}

// Test case: Script not found
TEST_F(UserInputTest, ScriptNotFound) {
    // Set invalid environment variable for script path
    setenv("FIREFLY_SCRIPT_PATH", "/non/existent/path.lua", 1);
    
    // Redirect stderr for testing
    testing::internal::CaptureStderr();
    
    // Create test arguments
    const char* args[] = {"program", "help"};
    
    // Call the function
    int result = processInput(2, const_cast<char**>(args));
    
    // Get captured error output
    std::string error_output = testing::internal::GetCapturedStderr();
    
    // Reset environment variable
    unsetenv("FIREFLY_SCRIPT_PATH");
    
    // Verify the result
    EXPECT_EQ(1, result);
    EXPECT_TRUE(error_output.find("Error: input_handler.lua not found") != std::string::npos);
}