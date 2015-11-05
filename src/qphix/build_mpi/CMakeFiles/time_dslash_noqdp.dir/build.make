# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.1

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
CMAKE_COMMAND = /cosma/local/cmake/3.1.3/bin/cmake

# The command to remove a file.
RM = /cosma/local/cmake/3.1.3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi

# Include any dependencies generated for this target.
include CMakeFiles/time_dslash_noqdp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/time_dslash_noqdp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/time_dslash_noqdp.dir/flags.make

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o: CMakeFiles/time_dslash_noqdp.dir/flags.make
CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o: ../tests/time_dslash_noqdp.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o -c /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/time_dslash_noqdp.cc

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.i"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC  $(CXX_DEFINES) $(CXX_FLAGS) -E /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/time_dslash_noqdp.cc > CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.i

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.s"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC  $(CXX_DEFINES) $(CXX_FLAGS) -S /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/time_dslash_noqdp.cc -o CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.s

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.requires:
.PHONY : CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.requires

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.provides: CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.requires
	$(MAKE) -f CMakeFiles/time_dslash_noqdp.dir/build.make CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.provides.build
.PHONY : CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.provides

CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.provides.build: CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o: CMakeFiles/time_dslash_noqdp.dir/flags.make
CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o: ../tests/timeDslashNoQDP.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o -c /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/timeDslashNoQDP.cc

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.i"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC  $(CXX_DEFINES) $(CXX_FLAGS) -E /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/timeDslashNoQDP.cc > CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.i

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.s"
	/cosma/local/platform_mpi/9.1.2/bin/mpiCC  $(CXX_DEFINES) $(CXX_FLAGS) -S /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/tests/timeDslashNoQDP.cc -o CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.s

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.requires:
.PHONY : CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.requires

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.provides: CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.requires
	$(MAKE) -f CMakeFiles/time_dslash_noqdp.dir/build.make CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.provides.build
.PHONY : CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.provides

CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.provides.build: CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o

# Object files for target time_dslash_noqdp
time_dslash_noqdp_OBJECTS = \
"CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o" \
"CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o"

# External object files for target time_dslash_noqdp
time_dslash_noqdp_EXTERNAL_OBJECTS =

time_dslash_noqdp: CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o
time_dslash_noqdp: CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o
time_dslash_noqdp: CMakeFiles/time_dslash_noqdp.dir/build.make
time_dslash_noqdp: lib/libqphix_solver.a
time_dslash_noqdp: /cosma5/data/dr002/dc-kash1/qmp/build/lib/libqmp.a
time_dslash_noqdp: /cosma/local/platform_mpi/9.1.2/lib/linux_amd64/libpcmpio.so
time_dslash_noqdp: /cosma/local/platform_mpi/9.1.2/lib/linux_amd64/libpcmpi.so
time_dslash_noqdp: /usr/lib64/libdl.so
time_dslash_noqdp: CMakeFiles/time_dslash_noqdp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable time_dslash_noqdp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/time_dslash_noqdp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/time_dslash_noqdp.dir/build: time_dslash_noqdp
.PHONY : CMakeFiles/time_dslash_noqdp.dir/build

CMakeFiles/time_dslash_noqdp.dir/requires: CMakeFiles/time_dslash_noqdp.dir/tests/time_dslash_noqdp.cc.o.requires
CMakeFiles/time_dslash_noqdp.dir/requires: CMakeFiles/time_dslash_noqdp.dir/tests/timeDslashNoQDP.cc.o.requires
.PHONY : CMakeFiles/time_dslash_noqdp.dir/requires

CMakeFiles/time_dslash_noqdp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/time_dslash_noqdp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/time_dslash_noqdp.dir/clean

CMakeFiles/time_dslash_noqdp.dir/depend:
	cd /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi /cosma5/data/dr002/dc-kash1/DiRAC3-testsuite/src/qphix/build_mpi/CMakeFiles/time_dslash_noqdp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/time_dslash_noqdp.dir/depend

