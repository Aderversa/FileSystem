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
CMAKE_SOURCE_DIR = /tmp/tmp.loZHbubqSI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04

# Include any dependencies generated for this target.
include src/library/CMakeFiles/FileSystemManager.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/library/CMakeFiles/FileSystemManager.dir/compiler_depend.make

# Include the progress variables for this target.
include src/library/CMakeFiles/FileSystemManager.dir/progress.make

# Include the compile flags for this target's objects.
include src/library/CMakeFiles/FileSystemManager.dir/flags.make

src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o: src/library/CMakeFiles/FileSystemManager.dir/flags.make
src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o: ../src/library/FileSystemManager.cpp
src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o: src/library/CMakeFiles/FileSystemManager.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o"
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o -MF CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o.d -o CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o -c /tmp/tmp.loZHbubqSI/src/library/FileSystemManager.cpp

src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.i"
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.loZHbubqSI/src/library/FileSystemManager.cpp > CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.i

src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.s"
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.loZHbubqSI/src/library/FileSystemManager.cpp -o CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.s

# Object files for target FileSystemManager
FileSystemManager_OBJECTS = \
"CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o"

# External object files for target FileSystemManager
FileSystemManager_EXTERNAL_OBJECTS =

src/library/libFileSystemManager.a: src/library/CMakeFiles/FileSystemManager.dir/FileSystemManager.cpp.o
src/library/libFileSystemManager.a: src/library/CMakeFiles/FileSystemManager.dir/build.make
src/library/libFileSystemManager.a: src/library/CMakeFiles/FileSystemManager.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libFileSystemManager.a"
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && $(CMAKE_COMMAND) -P CMakeFiles/FileSystemManager.dir/cmake_clean_target.cmake
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FileSystemManager.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/library/CMakeFiles/FileSystemManager.dir/build: src/library/libFileSystemManager.a
.PHONY : src/library/CMakeFiles/FileSystemManager.dir/build

src/library/CMakeFiles/FileSystemManager.dir/clean:
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library && $(CMAKE_COMMAND) -P CMakeFiles/FileSystemManager.dir/cmake_clean.cmake
.PHONY : src/library/CMakeFiles/FileSystemManager.dir/clean

src/library/CMakeFiles/FileSystemManager.dir/depend:
	cd /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.loZHbubqSI /tmp/tmp.loZHbubqSI/src/library /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04 /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library /tmp/tmp.loZHbubqSI/cmake-build-ubuntu22-04/src/library/CMakeFiles/FileSystemManager.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/library/CMakeFiles/FileSystemManager.dir/depend

