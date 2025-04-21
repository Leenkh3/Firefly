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