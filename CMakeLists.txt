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

# Directory for package searches
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Directory for executables
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

# Directory for outputs
SET(DIRAC3_TESTSUITE_OUTPUTS_PATH ${CMAKE_SOURCE_DIR}/outputs)

# Directory for scripts
SET(DIRAC3_TESTSUITE_SCRIPTS_PATH ${CMAKE_SOURCE_DIR}/scripts)

# Directory for module scripts
SET(DIRAC3_TESTSUITE_MODULES_PATH ${CMAKE_SOURCE_DIR}/modules)

# Directory for script templates
SET(DIRAC3_TESTSUITE_TEMPLATES_PATH ${CMAKE_SOURCE_DIR}/templates)

# Run directory
SET(DIRAC3_TESTSUITE_RUN_PATH ${CMAKE_SOURCE_DIR}/run)

# Find necessary packages and libraries
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_library(M_LIBRARIES m REQUIRED)

# Name of system
set(DIRAC3_HOST "archer")

# Add source
add_subdirectory(src)

ExternalProject_Add(swiftsim-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/swiftsim-benchmark.git
  GIT_TAG benchmark
  SOURCE_DIR ${CMAKE_BINARY_DIR}/swiftsim-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND libtoolize && ./autogen.sh && ./configure --prefix=${CMAKE_SOURCE_DIR} --with-metis
  BUILD_COMMAND make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/swiftsim-benchmark/examples && make install)
