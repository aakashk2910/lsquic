# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic

# Include any dependencies generated for this target.
include test/unittests/CMakeFiles/mini_parse.dir/depend.make

# Include the progress variables for this target.
include test/unittests/CMakeFiles/mini_parse.dir/progress.make

# Include the compile flags for this target's objects.
include test/unittests/CMakeFiles/mini_parse.dir/flags.make

test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.o: test/unittests/CMakeFiles/mini_parse.dir/flags.make
test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.o: test/unittests/mini_parse.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.o"
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mini_parse.dir/mini_parse.c.o   -c /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests/mini_parse.c

test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mini_parse.dir/mini_parse.c.i"
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests/mini_parse.c > CMakeFiles/mini_parse.dir/mini_parse.c.i

test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mini_parse.dir/mini_parse.c.s"
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests/mini_parse.c -o CMakeFiles/mini_parse.dir/mini_parse.c.s

# Object files for target mini_parse
mini_parse_OBJECTS = \
"CMakeFiles/mini_parse.dir/mini_parse.c.o"

# External object files for target mini_parse
mini_parse_EXTERNAL_OBJECTS =

test/unittests/mini_parse: test/unittests/CMakeFiles/mini_parse.dir/mini_parse.c.o
test/unittests/mini_parse: test/unittests/CMakeFiles/mini_parse.dir/build.make
test/unittests/mini_parse: src/liblsquic/liblsquic.a
test/unittests/mini_parse: /usr/local/lib/libevent.a
test/unittests/mini_parse: /Users/aakash/MastersThesisTUM/2020/boringssl/ssl/libssl.a
test/unittests/mini_parse: /Users/aakash/MastersThesisTUM/2020/boringssl/crypto/libcrypto.a
test/unittests/mini_parse: test/unittests/CMakeFiles/mini_parse.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable mini_parse"
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mini_parse.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/unittests/CMakeFiles/mini_parse.dir/build: test/unittests/mini_parse

.PHONY : test/unittests/CMakeFiles/mini_parse.dir/build

test/unittests/CMakeFiles/mini_parse.dir/clean:
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests && $(CMAKE_COMMAND) -P CMakeFiles/mini_parse.dir/cmake_clean.cmake
.PHONY : test/unittests/CMakeFiles/mini_parse.dir/clean

test/unittests/CMakeFiles/mini_parse.dir/depend:
	cd /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests /Users/aakash/MastersThesisTUM/2020/boringssl/lsquic/test/unittests/CMakeFiles/mini_parse.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/unittests/CMakeFiles/mini_parse.dir/depend

