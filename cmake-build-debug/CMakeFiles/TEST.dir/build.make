# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/luman6666sinacom/Documents/Y86-Conc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TEST.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TEST.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TEST.dir/flags.make

CMakeFiles/TEST.dir/test.cpp.o: CMakeFiles/TEST.dir/flags.make
CMakeFiles/TEST.dir/test.cpp.o: ../test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TEST.dir/test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TEST.dir/test.cpp.o -c /Users/luman6666sinacom/Documents/Y86-Conc/test.cpp

CMakeFiles/TEST.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TEST.dir/test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/luman6666sinacom/Documents/Y86-Conc/test.cpp > CMakeFiles/TEST.dir/test.cpp.i

CMakeFiles/TEST.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TEST.dir/test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/luman6666sinacom/Documents/Y86-Conc/test.cpp -o CMakeFiles/TEST.dir/test.cpp.s

CMakeFiles/TEST.dir/test.cpp.o.requires:

.PHONY : CMakeFiles/TEST.dir/test.cpp.o.requires

CMakeFiles/TEST.dir/test.cpp.o.provides: CMakeFiles/TEST.dir/test.cpp.o.requires
	$(MAKE) -f CMakeFiles/TEST.dir/build.make CMakeFiles/TEST.dir/test.cpp.o.provides.build
.PHONY : CMakeFiles/TEST.dir/test.cpp.o.provides

CMakeFiles/TEST.dir/test.cpp.o.provides.build: CMakeFiles/TEST.dir/test.cpp.o


# Object files for target TEST
TEST_OBJECTS = \
"CMakeFiles/TEST.dir/test.cpp.o"

# External object files for target TEST
TEST_EXTERNAL_OBJECTS =

../TEST: CMakeFiles/TEST.dir/test.cpp.o
../TEST: CMakeFiles/TEST.dir/build.make
../TEST: CMakeFiles/TEST.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../TEST"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TEST.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TEST.dir/build: ../TEST

.PHONY : CMakeFiles/TEST.dir/build

CMakeFiles/TEST.dir/requires: CMakeFiles/TEST.dir/test.cpp.o.requires

.PHONY : CMakeFiles/TEST.dir/requires

CMakeFiles/TEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TEST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TEST.dir/clean

CMakeFiles/TEST.dir/depend:
	cd /Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/luman6666sinacom/Documents/Y86-Conc /Users/luman6666sinacom/Documents/Y86-Conc /Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug /Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug /Users/luman6666sinacom/Documents/Y86-Conc/cmake-build-debug/CMakeFiles/TEST.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TEST.dir/depend

