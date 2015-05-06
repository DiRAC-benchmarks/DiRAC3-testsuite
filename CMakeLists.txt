cmake_minimum_required(VERSION 2.8)
project(DiRAC3-testsuite C)

SET(DIRAC3_TESTSUITE_VERSION_MAJOR 0)
SET(DIRAC3_TESTSUITE_VERSION_MINOR 1)

# Guard against in-source builds
get_filename_component(SRCDIR "${CMAKE_SOURCE_DIR}" REALPATH)
get_filename_component(BINDIR "${CMAKE_BINARY_DIR}" REALPATH)
if("${SRCDIR}" STREQUAL "${BINDIR}")
	message("")
	message("Warning: In-source builds disabled.")
	message("Run CMake in a separate build directory.")
	message("You will need to remove the CMakeCache.txt file")
	message("and /CMakeFiles directory before proceeding.")
	message(FATAL_ERROR "Terminating CMake.")
endif()

# Directory for package searches
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Directory for executables
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)
