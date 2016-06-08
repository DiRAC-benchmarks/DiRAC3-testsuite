cmake_minimum_required(VERSION 3.0)
project(DiRAC3-testsuite C CXX Fortran)

include(ExternalProject)

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

# Define various paths
set(CMAKE_MODULE_PATH               ${PROJECT_SOURCE_DIR}/cmake)
SET(EXECUTABLE_OUTPUT_PATH          ${PROJECT_SOURCE_DIR}/bin)
SET(DIRAC3_TESTSUITE_MODULES_PATH   ${PROJECT_SOURCE_DIR}/modules)
SET(DIRAC3_TESTSUITE_OUTPUTS_PATH   ${PROJECT_SOURCE_DIR}/outputs)
SET(DIRAC3_TESTSUITE_RUN_PATH       ${PROJECT_SOURCE_DIR}/run)
SET(DIRAC3_TESTSUITE_SCRIPTS_PATH   ${PROJECT_SOURCE_DIR}/scripts)
SET(DIRAC3_TESTSUITE_TEMPLATES_PATH ${PROJECT_SOURCE_DIR}/templates)

# Find necessary packages and libraries
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_library(M_LIBRARIES m REQUIRED)

# Default system name
set(DIRAC3_HOST "default")

# Add source
add_subdirectory(src)
