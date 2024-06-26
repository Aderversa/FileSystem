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
include src/library/CMakeFiles/Disk.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/library/CMakeFiles/Disk.dir/compiler_depend.make

# Include the progress variables for this target.
include src/library/CMakeFiles/Disk.dir/progress.make

# Include the compile flags for this target's objects.
include src/library/CMakeFiles/Disk.dir/flags.make

src/library/CMakeFiles/Disk.dir/disk.cpp.o: src/library/CMakeFiles/Disk.dir/flags.make
src/library/CMakeFiles/Disk.dir/disk.cpp.o: ../src/library/disk.cpp
src/library/CMakeFiles/Disk.dir/disk.cpp.o: src/library/CMakeFiles/Disk.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aderversa/project_study/fileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/library/CMakeFiles/Disk.dir/disk.cpp.o"
	cd /home/aderversa/project_study/fileSystem/build/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/library/CMakeFiles/Disk.dir/disk.cpp.o -MF CMakeFiles/Disk.dir/disk.cpp.o.d -o CMakeFiles/Disk.dir/disk.cpp.o -c /home/aderversa/project_study/fileSystem/src/library/disk.cpp

src/library/CMakeFiles/Disk.dir/disk.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Disk.dir/disk.cpp.i"
	cd /home/aderversa/project_study/fileSystem/build/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aderversa/project_study/fileSystem/src/library/disk.cpp > CMakeFiles/Disk.dir/disk.cpp.i

src/library/CMakeFiles/Disk.dir/disk.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Disk.dir/disk.cpp.s"
	cd /home/aderversa/project_study/fileSystem/build/src/library && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aderversa/project_study/fileSystem/src/library/disk.cpp -o CMakeFiles/Disk.dir/disk.cpp.s

# Object files for target Disk
Disk_OBJECTS = \
"CMakeFiles/Disk.dir/disk.cpp.o"

# External object files for target Disk
Disk_EXTERNAL_OBJECTS =

src/library/libDisk.a: src/library/CMakeFiles/Disk.dir/disk.cpp.o
src/library/libDisk.a: src/library/CMakeFiles/Disk.dir/build.make
src/library/libDisk.a: src/library/CMakeFiles/Disk.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aderversa/project_study/fileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libDisk.a"
	cd /home/aderversa/project_study/fileSystem/build/src/library && $(CMAKE_COMMAND) -P CMakeFiles/Disk.dir/cmake_clean_target.cmake
	cd /home/aderversa/project_study/fileSystem/build/src/library && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Disk.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/library/CMakeFiles/Disk.dir/build: src/library/libDisk.a
.PHONY : src/library/CMakeFiles/Disk.dir/build

src/library/CMakeFiles/Disk.dir/clean:
	cd /home/aderversa/project_study/fileSystem/build/src/library && $(CMAKE_COMMAND) -P CMakeFiles/Disk.dir/cmake_clean.cmake
.PHONY : src/library/CMakeFiles/Disk.dir/clean

src/library/CMakeFiles/Disk.dir/depend:
	cd /home/aderversa/project_study/fileSystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aderversa/project_study/fileSystem /home/aderversa/project_study/fileSystem/src/library /home/aderversa/project_study/fileSystem/build /home/aderversa/project_study/fileSystem/build/src/library /home/aderversa/project_study/fileSystem/build/src/library/CMakeFiles/Disk.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/library/CMakeFiles/Disk.dir/depend

