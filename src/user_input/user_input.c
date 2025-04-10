#include "user_input.h"


// Function definitions
int processInput(int argc, char *argv[]) {

    printf("Starting program...\n");

    ///
    /// BEGIN
    /// BEGIN OF USER INPUT SECTION!

    // This section is for user input and processing
    // It can be replaced with a Lua script for interactive input
    // or for processing arguments directly
    
     // Initialize Lua
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // Load the Lua script
    // Check if the script path is provided as an environment variable
    // If not, use a default path
    const char* script_path = getenv("FIREFLY_SCRIPT_PATH");
    if (script_path == NULL) {
        script_path = "./src/user_input/input_handler.lua";  // Default fallback
    }
    printf("Loading Lua script from: %s\n", script_path);

    // Check if the Lua script exists
    // If not, prompt the user to create it from a template
    // and exit the program
    FILE* test = fopen(script_path, "r");
    if (test == NULL) {
        fprintf(stderr, "Error: input_handler.lua not found at %s!\n", script_path);
        fprintf(stderr, "Create it from template: cp ./src/user_input/input_handler.template.lua ./src/user_input/input_handler.lua\n");
        lua_close(L);
        return 1;
    }
    fclose(test);

    // This will load the script into the Lua state
    // and prepare it for execution
    // The script should define the functions get_user_input and process_arguments
    int status = luaL_dofile(L, script_path);

    // Check for errors in loading the Lua script
    if (status) {
        fprintf(stderr, "Couldn't load Lua script: %s\n", lua_tostring(L, -1));
        lua_close(L);
        return 1;
    }

    // If arguments were provided, process them directly
    if (argc > 1) {
        printf("Arguments provided, processing directly...\n");
        
        // Call the Lua function to process arguments
        lua_getglobal(L, "process_arguments");
        
        // Create a Lua table to hold the arguments
        lua_createtable(L, argc, 0);
        
        // Fill the table with command-line arguments
        for (int i = 0; i < argc; i++) {
            lua_pushstring(L, argv[i]);
            lua_rawseti(L, -2, i + 1); // Lua arrays are 1-based
        }
        
        // Call the function with the arguments table
        if (lua_pcall(L, 1, 3, 0) != 0) {
            fprintf(stderr, "Error running Lua function: %s\n", lua_tostring(L, -1));
            lua_close(L);
            return 1;
        }
    } else {
        // No arguments provided, use Lua for interactive input
        printf("No arguments provided, starting interactive mode...\n");
        
        // Call the main Lua function for interactive mode
        lua_getglobal(L, "get_user_input");
        if (lua_pcall(L, 0, 3, 0) != 0) {
            fprintf(stderr, "Error running Lua function: %s\n", lua_tostring(L, -1));
            lua_close(L);
            return 1;
        }
    }
    
    // Get results from Lua
    const char *operation = lua_tostring(L, -3);
    const char *filetype = lua_tostring(L, -2);
    const char *filename = lua_tostring(L, -1);

    printf("\nReceived from Lua:\n");
    printf("Operation: %s\n", operation);

    if (strcmp(operation, "help") != 0 && strcmp(operation, "exit") != 0) {
        printf("Filename: %s\n", filename);
        printf("Filetype: %s\n", filetype);
        
        // Process the file based on operation and filetype
        if (strcmp(operation, "load") == 0) {
            printf("Processing file load for %s file: %s\n", filetype, filename);
            // File load implementation
        } else if (strcmp(operation, "export") == 0) {
            printf("Processing file export for %s file: %s\n", filetype, filename);
            // File export implementation
        }
    } else if (strcmp(operation, "exit") == 0) {
        printf("Exiting program...\n");
    } else {
        // Help operation was already displayed in Lua, nothing to do here
    }
    
    // Clean up Lua
    lua_close(L);

    /// END
    /// END OF USER INPUT SECTION!
    /// 


    printf("Program finished.\n");
    return 0;
}
