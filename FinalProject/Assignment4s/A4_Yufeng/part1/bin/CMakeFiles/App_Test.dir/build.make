# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin

# Include any dependencies generated for this target.
include CMakeFiles/App_Test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/App_Test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/App_Test.dir/flags.make

CMakeFiles/App_Test.dir/src/App.cpp.o: CMakeFiles/App_Test.dir/flags.make
CMakeFiles/App_Test.dir/src/App.cpp.o: ../src/App.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/App_Test.dir/src/App.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/App_Test.dir/src/App.cpp.o -c /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/App.cpp

CMakeFiles/App_Test.dir/src/App.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_Test.dir/src/App.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/App.cpp > CMakeFiles/App_Test.dir/src/App.cpp.i

CMakeFiles/App_Test.dir/src/App.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_Test.dir/src/App.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/App.cpp -o CMakeFiles/App_Test.dir/src/App.cpp.s

CMakeFiles/App_Test.dir/src/Command.cpp.o: CMakeFiles/App_Test.dir/flags.make
CMakeFiles/App_Test.dir/src/Command.cpp.o: ../src/Command.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/App_Test.dir/src/Command.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/App_Test.dir/src/Command.cpp.o -c /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Command.cpp

CMakeFiles/App_Test.dir/src/Command.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_Test.dir/src/Command.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Command.cpp > CMakeFiles/App_Test.dir/src/Command.cpp.i

CMakeFiles/App_Test.dir/src/Command.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_Test.dir/src/Command.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Command.cpp -o CMakeFiles/App_Test.dir/src/Command.cpp.s

CMakeFiles/App_Test.dir/src/Draw.cpp.o: CMakeFiles/App_Test.dir/flags.make
CMakeFiles/App_Test.dir/src/Draw.cpp.o: ../src/Draw.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/App_Test.dir/src/Draw.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/App_Test.dir/src/Draw.cpp.o -c /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Draw.cpp

CMakeFiles/App_Test.dir/src/Draw.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_Test.dir/src/Draw.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Draw.cpp > CMakeFiles/App_Test.dir/src/Draw.cpp.i

CMakeFiles/App_Test.dir/src/Draw.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_Test.dir/src/Draw.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/src/Draw.cpp -o CMakeFiles/App_Test.dir/src/Draw.cpp.s

CMakeFiles/App_Test.dir/tests/main_test.cpp.o: CMakeFiles/App_Test.dir/flags.make
CMakeFiles/App_Test.dir/tests/main_test.cpp.o: ../tests/main_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/App_Test.dir/tests/main_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/App_Test.dir/tests/main_test.cpp.o -c /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/main_test.cpp

CMakeFiles/App_Test.dir/tests/main_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_Test.dir/tests/main_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/main_test.cpp > CMakeFiles/App_Test.dir/tests/main_test.cpp.i

CMakeFiles/App_Test.dir/tests/main_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_Test.dir/tests/main_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/main_test.cpp -o CMakeFiles/App_Test.dir/tests/main_test.cpp.s

CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o: CMakeFiles/App_Test.dir/flags.make
CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o: ../tests/catch_amalgamated.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o -c /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/catch_amalgamated.cpp

CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/catch_amalgamated.cpp > CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.i

CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/tests/catch_amalgamated.cpp -o CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.s

# Object files for target App_Test
App_Test_OBJECTS = \
"CMakeFiles/App_Test.dir/src/App.cpp.o" \
"CMakeFiles/App_Test.dir/src/Command.cpp.o" \
"CMakeFiles/App_Test.dir/src/Draw.cpp.o" \
"CMakeFiles/App_Test.dir/tests/main_test.cpp.o" \
"CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o"

# External object files for target App_Test
App_Test_EXTERNAL_OBJECTS =

App_Test: CMakeFiles/App_Test.dir/src/App.cpp.o
App_Test: CMakeFiles/App_Test.dir/src/Command.cpp.o
App_Test: CMakeFiles/App_Test.dir/src/Draw.cpp.o
App_Test: CMakeFiles/App_Test.dir/tests/main_test.cpp.o
App_Test: CMakeFiles/App_Test.dir/tests/catch_amalgamated.cpp.o
App_Test: CMakeFiles/App_Test.dir/build.make
App_Test: CMakeFiles/App_Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable App_Test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/App_Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/App_Test.dir/build: App_Test

.PHONY : CMakeFiles/App_Test.dir/build

CMakeFiles/App_Test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/App_Test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/App_Test.dir/clean

CMakeFiles/App_Test.dir/depend:
	cd /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1 /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1 /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin /home/george/CS5500/monorepo-yufeng-gao/Assignment4_Testing/part1/bin/CMakeFiles/App_Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/App_Test.dir/depend
