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

-- For debugging, print that the script loaded
print("Test Lua script loaded successfully")