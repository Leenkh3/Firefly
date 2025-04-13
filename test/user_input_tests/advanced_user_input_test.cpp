#include <gtest/gtest.h>
#include <fstream>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "user_input/user_input.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

class AdvancedUserInputTest : public ::testing::Test {
protected:
    lua_State *L;
    
    void SetUp() override {
        // Create advanced test script file
        createAdvancedTestScript();
        
        // Set environment variable for the test script
        setenv("FIREFLY_SCRIPT_PATH", "./advanced_test_script.lua", 1);
        
        // Initialize Lua state for direct script manipulation
        L = luaL_newstate();
        luaL_openlibs(L);
        luaL_dofile(L, "./advanced_test_script.lua");
    }

    void TearDown() override {
        // Clean up Lua state
        if (L) lua_close(L);
        
        // Clean up test files
        std::remove("./advanced_test_script.lua");
        
        // Reset environment variable
        unsetenv("FIREFLY_SCRIPT_PATH");
    }

    void createAdvancedTestScript() {
        std::ofstream scriptFile("./advanced_test_script.lua");
        scriptFile << R"(
-- Advanced test input handler script

-- Global variable to track function calls
test_results = {
    process_args_called = false,
    interactive_called = false,
    last_args = {},
    error_simulation = false
}

-- Function to process command line arguments
function process_arguments(args)
    test_results.process_args_called = true
    test_results.last_args = args
    
    -- Check if we should simulate an error
    if test_results.error_simulation then
        error("Simulated error in process_arguments")
    end
    
    -- Process the arguments
    local operation = "unknown"
    local filetype = ""
    local filename = ""
    
    if args[2] == "load" and args[3] then
        operation = "load"
        filename = args[3]
        
        -- Extract filetype from filename extension
        local ext = string.match(filename, "%.(%w+)$")
        if ext then
            filetype = ext
        else
            filetype = "unknown"
        end
    elseif args[2] == "export" and args[3] and args[4] then
        operation = "export"
        filetype = args[3]
        filename = args[4]
    elseif args[2] == "help" then
        operation = "help"
        print("Help: Available commands are load, export, help, exit")
    elseif args[2] == "exit" then
        operation = "exit"
    end
    
    return operation, filetype, filename
end

-- Function for interactive user input
function get_user_input()
    test_results.interactive_called = true
    
    -- Check if we should simulate an error
    if test_results.error_simulation then
        error("Simulated error in get_user_input")
    end
    
    -- In a real scenario, this would prompt the user
    -- For testing, we'll just return predefined values
    print("Interactive mode: Please choose an operation (load, export, help, exit):")
    print("Selected: load")
    
    print("Enter filename:")
    print("Selected: test_data.csv")
    
    local operation = "load"
    local filename = "test_data.csv"
    local filetype = "csv"
    
    return operation, filetype, filename
end

-- Additional function to set up test conditions
function setup_test(config)
    if config.error then
        test_results.error_simulation = true
    else
        test_results.error_simulation = false
    end
    
    test_results.process_args_called = false
    test_results.interactive_called = false
    test_results.last_args = {}
    
    return "setup_complete"
end
)";
        scriptFile.close();
    }
    
    // Helper method to reset test state in Lua
    void resetTestState(bool simulateError = false) {
        lua_getglobal(L, "setup_test");
        lua_newtable(L);
        lua_pushboolean(L, simulateError);
        lua_setfield(L, -2, "error");
        lua_call(L, 1, 1);
        lua_pop(L, 1);
    }
    
    // Helper method to check if a function was called
    bool wasFunctionCalled(const char* functionType) {
        lua_getglobal(L, "test_results");
        lua_getfield(L, -1, functionType);
        bool result = lua_toboolean(L, -1);
        lua_pop(L, 2);
        return result;
    }
};

// Test with load command and filename
TEST_F(AdvancedUserInputTest, ProcessLoadWithFilename) {
    resetTestState();
    
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments
    const char* args[] = {"program", "load", "data.csv"};
    
    // Call the function
    int result = processInput(3, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(wasFunctionCalled("process_args_called"));
    EXPECT_FALSE(wasFunctionCalled("interactive_called"));
    EXPECT_TRUE(output.find("Operation: load") != std::string::npos);
    EXPECT_TRUE(output.find("Filetype: csv") != std::string::npos);
    EXPECT_TRUE(output.find("Filename: data.csv") != std::string::npos);
}

// Test export with filetype and filename
TEST_F(AdvancedUserInputTest, ProcessExportWithFiletypeAndFilename) {
    resetTestState();
    
    // Redirect stdout for testing
    testing::internal::CaptureStdout();
    
    // Create test arguments
    const char* args[] = {"program", "export", "json", "output.json"};
    
    // Call the function
    int result = processInput(4, const_cast<char**>(args));
    
    // Get captured output
    std::string output = testing::internal::GetCapturedStdout();
    
    // Verify the result
    EXPECT_EQ(0, result);
    EXPECT_TRUE(wasFunctionCalled("process_args_called"));
    EXPECT_FALSE(wasFunctionCalled("interactive_called"));
    EXPECT_TRUE(output.find("Operation: export") != std::string::npos);
    EXPECT_TRUE(output.find("Filetype: json") != std::string::npos);
    EXPECT_TRUE(output.find("Filename: output.json") != std::string::npos);
}

// Test error handling in Lua function
TEST_F(AdvancedUserInputTest, HandleLuaFunctionError) {
    resetTestState(true); // Simulate error
    
    // Redirect stderr for testing
    testing::internal::CaptureStderr();
    
    // Create test arguments
    const char* args[] = {"program", "load", "data.csv"};
    
    // Call the function
    int result = processInput(3, const_cast<char**>(args));
    
    // Get captured error output
    std::string error_output = testing::internal::GetCapturedStderr();
    
    // Verify the result
    EXPECT_EQ(1, result);
    EXPECT_TRUE(error_output.find("Error running Lua function") != std::string::npos);
    EXPECT_TRUE(error_output.find("Simulated error in process_arguments") != std::string::npos);
}

// Test interactive mode error handling
TEST_F(AdvancedUserInputTest, HandleInteractiveModeError) {
    resetTestState(true); // Simulate error
    
    // Redirect stderr for testing
    testing::internal::CaptureStderr();
    
    // Create test arguments (just program name to trigger interactive mode)
    const char* args[] = {"program"};
    
    // Call the function
    int result = processInput(1, const_cast<char**>(args));
    
    // Get captured error output
    std::string error_output = testing::internal::GetCapturedStderr();
    
    // Verify the result
    EXPECT_EQ(1, result);
    EXPECT_TRUE(error_output.find("Error running Lua function") != std::string::npos);
    EXPECT_TRUE(error_output.find("Simulated error in get_user_input") != std::string::npos);
}