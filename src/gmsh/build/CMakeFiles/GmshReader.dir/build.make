# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /work/students/hpc-f0v113/cmake/bin/cmake

# The command to remove a file.
RM = /work/students/hpc-f0v113/cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /work/students/hpc-f0v113/firefly/Firefly/CmakeTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build

# Include any dependencies generated for this target.
include CMakeFiles/GmshReader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GmshReader.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GmshReader.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GmshReader.dir/flags.make

CMakeFiles/GmshReader.dir/gmsh_reader.c.o: CMakeFiles/GmshReader.dir/flags.make
CMakeFiles/GmshReader.dir/gmsh_reader.c.o: /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/gmsh_reader.c
CMakeFiles/GmshReader.dir/gmsh_reader.c.o: CMakeFiles/GmshReader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/GmshReader.dir/gmsh_reader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/GmshReader.dir/gmsh_reader.c.o -MF CMakeFiles/GmshReader.dir/gmsh_reader.c.o.d -o CMakeFiles/GmshReader.dir/gmsh_reader.c.o -c /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/gmsh_reader.c

CMakeFiles/GmshReader.dir/gmsh_reader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/GmshReader.dir/gmsh_reader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/gmsh_reader.c > CMakeFiles/GmshReader.dir/gmsh_reader.c.i

CMakeFiles/GmshReader.dir/gmsh_reader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/GmshReader.dir/gmsh_reader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/gmsh_reader.c -o CMakeFiles/GmshReader.dir/gmsh_reader.c.s

# Object files for target GmshReader
GmshReader_OBJECTS = \
"CMakeFiles/GmshReader.dir/gmsh_reader.c.o"

# External object files for target GmshReader
GmshReader_EXTERNAL_OBJECTS =

GmshReader: CMakeFiles/GmshReader.dir/gmsh_reader.c.o
GmshReader: CMakeFiles/GmshReader.dir/build.make
GmshReader: CMakeFiles/GmshReader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable GmshReader"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GmshReader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GmshReader.dir/build: GmshReader
.PHONY : CMakeFiles/GmshReader.dir/build

CMakeFiles/GmshReader.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GmshReader.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GmshReader.dir/clean

CMakeFiles/GmshReader.dir/depend:
	cd /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /work/students/hpc-f0v113/firefly/Firefly/CmakeTest /work/students/hpc-f0v113/firefly/Firefly/CmakeTest /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build /work/students/hpc-f0v113/firefly/Firefly/CmakeTest/build/CMakeFiles/GmshReader.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GmshReader.dir/depend

