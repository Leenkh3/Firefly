-- Function to process command-line arguments
_G.process_arguments = function(args)
    local operation = ""
    local filename = ""
    local filetype = ""
    
    -- Parse the arguments
    for i = 2, #args do  -- Start from 2 to skip program name
        local arg = args[i]
        
        if arg == "--load" then
            operation = "load"
        elseif arg == "--export" then
            operation = "export"
        elseif arg == "--help" then
            operation = "help"
            
            -- Display help text directly
            print("\nProgram usage:")
            print("  ./program --load <filename> --type <filetype>")
            print("  ./program --export <filename> --type <filetype>")
            print("  ./program --help")
            print("Supported file types: txt, gmsh, json")
            
            return "help", "", ""
        elseif arg == "--type" and i < #args then
            filetype = args[i + 1]
        elseif not arg:match("^%-%-") and operation ~= "" and filename == "" then
            -- If it's not a flag and we already have an operation but no filename
            filename = arg
        end
    end
    
    -- Validate inputs
    if operation == "" then
        print("Error: No operation specified (--load, --export, or --help)")
        return "error", "", ""
    end
    
    if operation ~= "help" and filename == "" then
        print("Error: No filename specified")
        return "error", "", ""
    end
    
    if operation ~= "help" and filetype == "" then
        print("Error: No filetype specified (--type txt|gmsh|json)")
        return "error", "", ""
    end
    
    -- Return the parsed information
    return operation, filename, filetype
end

-- Explicitly define the function in _G (global environment) to ensure it's accessible
_G.get_user_input = function()
    print("\n=== Firefly ===")
    
    while true do
        -- Ask for operation type
        print("\nSelect operation:")
        print("1. File load (loads a file)")
        print("2. File export (save to a file)")
        print("3. Help")
        print("4. Exit")
        
        local choice = ""
        repeat
            io.write("Enter your choice (1-4): ")
            io.flush()
            choice = io.read("*l")
            
            if choice == "1" then
                choice = "load"
                break
            elseif choice == "2" then
                choice = "export"
                break
            elseif choice == "3" then
                -- Display help text directly in Lua
                print("\nProgram usage:")
                print("  ./program --load <filename> --type <filetype>")
                print("  ./program --export <filename> --type <filetype>")
                print("  ./program --help")
                print("Supported file types: txt, gmsh, json")
                
                -- Continue the loop instead of returning
                choice = ""
                break
            elseif choice == "4" then
                return "exit", "", ""
            else
                print("Invalid choice. Please try again.")
            end
        until false
        
        -- If help was selected or an invalid choice was made, continue the loop
        if choice == "" then
            -- Just continue the loop
        else
            -- Ask for file type
            print("\nSelect file type:")
            print("1. txt")
            print("2. gmsh")
            print("3. json")
            
            local filetype = ""
            repeat
                io.write("Enter your choice (1-3): ")
                io.flush()
                local type_choice = io.read("*l")
                
                if type_choice == "1" then
                    filetype = "txt"
                    break
                elseif type_choice == "2" then
                    filetype = "gmsh"
                    break
                elseif type_choice == "3" then
                    filetype = "json"
                    break
                else
                    print("Invalid choice. Please try again.")
                end
            until false

            -- Ask for filename
            local filename = ""
            repeat
                io.write("\nEnter filename: ")
                io.flush()
                filename = io.read("*l")
                
                if filename == "" then
                    print("Filename cannot be empty. Please try again.")
                end
            until filename ~= ""
            
            -- Return the collected information to C
            return choice, filetype, filename
        end
    end
end

-- For debugging, print that the script loaded
print("Lua script loaded successfully")
print("get_user_input function is defined: " .. tostring(_G.get_user_input ~= nil))
print("process_arguments function is defined: " .. tostring(_G.process_arguments ~= nil))