# User Input Module Development Guide

This guide explains how to set up your development environment for working with the User Input module and its Lua integration.

## Initial Setup

Before developing the User Input module, you need to build Lua:

```bash
mkdir -p build
cd build
cmake ..
make lua
```

This will download, build, and install Lua headers to `src/user_input/LUA/include` and libraries to `src/user_input/LUA/lib`.

## Development Workflow

Once Lua is built, use this command for your regular development cycle:

```bash
cd build
make user_input
```

Note: The `user_input` target depends on the `lua` target, so building `user_input` will automatically build Lua if needed.

## Development Tips

### IDE Configuration
- Add `src/user_input/LUA/include` to your IDE's include paths to get proper code completion and error checking

### Testing and Debugging
- Ensure the Lua script `src/user_input/input_handler.lua` exists
- When running the binary, make sure your working directory is set correctly so the relative path to the Lua script works

### CMake Configuration
When modifying the build system, note that Lua is built with:
```bash
BUILD_COMMAND make MYCFLAGS=-fPIC linux
```
This builds Lua for Linux with position-independent code.