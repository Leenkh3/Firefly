# This script selects the appropriate Lua build target based on platform

message(STATUS "Building Lua for platform: ${CMAKE_SYSTEM_NAME}")

# Select the appropriate make target based on platform
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    execute_process(COMMAND make MYCFLAGS=-fPIC macosx)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    execute_process(COMMAND make MYCFLAGS=-fPIC linux)
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    execute_process(COMMAND make MYCFLAGS=-fPIC freebsd)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if(DEFINED ENV{MSYSTEM} AND "$ENV{MSYSTEM}" STREQUAL "MINGW64")
        execute_process(COMMAND make MYCFLAGS=-fPIC mingw)
    else()
        execute_process(COMMAND make MYCFLAGS=-fPIC generic)
    endif()
else()
    # Default to a more generic POSIX target for other platforms
    message(STATUS "Using generic posix target for ${CMAKE_SYSTEM_NAME}")
    execute_process(COMMAND make MYCFLAGS=-fPIC posix)
endif()