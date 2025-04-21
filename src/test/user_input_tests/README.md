# User Input Testing Documentation

This document explains how to test the user input functionality of the Firefly project.

## Overview

The user input system in Firefly is designed to process command-line arguments and provide an interactive mode for user input. It uses Lua scripting to handle the input processing logic, making it highly customizable.

## Key Components

1. **processInput()** function in `user_input.c` - The main entry point for handling user input
2. **Lua Scripts** - Scripts that define the actual input handling logic
3. **Unit Tests** - Tests that verify the functionality works correctly

## Input Handling Methods

The system supports two main input methods:

1. **Command-line arguments** - Passing arguments directly when running the program
2. **Interactive mode** - Starting the program without arguments and entering information interactively

## Testing

The unit tests verify that:

1. The system properly loads and executes Lua scripts
2. Command-line arguments are correctly passed to Lua functions
3. Interactive mode works as expected
4. Error handling works correctly
5. The system returns appropriate values for different inputs

## Test Cases

1. **Basic Command Processing**
   - Testing help command
   - Testing load command with a file
   - Testing export command with filetype and filename
   - Testing exit command

2. **Advanced Functionality**
   - Testing with custom file formats
   - Testing error handling
   - Testing interactive mode behavior

3. **Error Handling**
   - Missing script file
   - Lua execution errors
   - Invalid arguments

## Extending the Tests

To add more tests:

1. Create new test methods in the test files
2. Create or modify Lua scripts to test specific functionality
3. Update the CMake files if necessary

## User Input Usage Example

```c
// Example of using the user input functionality:
#include "user_input/user_input.h"

int main(int argc, char *argv[]) {
    // Process command-line arguments or start interactive mode
    int result = processInput(argc, argv);
    
    if (result != 0) {
        // Handle error
        return result;
    }
    
    // Continue with program execution
    return 0;
}
```