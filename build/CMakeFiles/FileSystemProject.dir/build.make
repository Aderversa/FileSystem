# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aderversa/project_study/fileSystem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aderversa/project_study/fileSystem/build

# Include any dependencies generated for this target.
include CMakeFiles/FileSystemProject.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/FileSystemProject.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/FileSystemProject.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FileSystemProject.dir/flags.make

CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o: CMakeFiles/FileSystemProject.dir/flags.make
CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o: ../src/shell/sfssh.cpp
CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o: CMakeFiles/FileSystemProject.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aderversa/project_study/fileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o -MF CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o.d -o CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o -c /home/aderversa/project_study/fileSystem/src/shell/sfssh.cpp

CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aderversa/project_study/fileSystem/src/shell/sfssh.cpp > CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.i

CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aderversa/project_study/fileSystem/src/shell/sfssh.cpp -o CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.s

# Object files for target FileSystemProject
FileSystemProject_OBJECTS = \
"CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o"

# External object files for target FileSystemProject
FileSystemProject_EXTERNAL_OBJECTS =

FileSystemProject: CMakeFiles/FileSystemProject.dir/src/shell/sfssh.cpp.o
FileSystemProject: CMakeFiles/FileSystemProject.dir/build.make
FileSystemProject: src/library/libFileSystem.a
FileSystemProject: src/library/libDisk.a
FileSystemProject: CMakeFiles/FileSystemProject.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aderversa/project_study/fileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FileSystemProject"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileSystemProject.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FileSystemProject.dir/build: FileSystemProject
.PHONY : CMakeFiles/FileSystemProject.dir/build

CMakeFiles/FileSystemProject.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FileSystemProject.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FileSystemProject.dir/clean

CMakeFiles/FileSystemProject.dir/depend:
	cd /home/aderversa/project_study/fileSystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aderversa/project_study/fileSystem /home/aderversa/project_study/fileSystem /home/aderversa/project_study/fileSystem/build /home/aderversa/project_study/fileSystem/build /home/aderversa/project_study/fileSystem/build/CMakeFiles/FileSystemProject.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FileSystemProject.dir/depend

