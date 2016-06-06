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

# Grid Benchmark
ExternalProject_Add(grid-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:paboyle/Grid
  GIT_TAG master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/grid-benchmark
  BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ./scripts/reconfigure_script
		&& sed -i "s/lib tests benchmarks/lib benchmarks/g" Makefile.am
		&& mkdir -p builds/icpc-avx-openmp
		&& cd builds/icpc-avx-openmp
		&& CXX=icpc ../../configure --enable-precision=single --enable-simd=AVX "CXXFLAGS=-mavx -fopenmp -O3 -std=c++11" "LIBS=-fopenmp -lgmp -lmpfr" --enable-comms=none --prefix=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/icpc-avx-openmp && make all
	INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/icpc-avx-openmp && make install)

if(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)

	# Script defining modules required by Walls
	set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.grid.${DIRAC3_HOST}")

	# Run and submission scripts for Walls3D
	set(JOB_NAME       "GridSingleNode")
	set(SCRIPT_RUN     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.grid.${DIRAC3_HOST}")
	set(SCRIPT_RUN_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.grid.in")
	set(SCRIPT_SUB     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.grid.${DIRAC3_HOST}")
	set(SCRIPT_SUB_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
	configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
	configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
	file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
	file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})

endif(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)

# Swift Benchmark
ExternalProject_Add(swiftsim-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/swiftsim-benchmark.git
  GIT_TAG benchmark
  SOURCE_DIR ${CMAKE_BINARY_DIR}/swiftsim-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND libtoolize && ./autogen.sh && ./configure --prefix=${CMAKE_SOURCE_DIR} --with-metis
  BUILD_COMMAND make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/swiftsim-benchmark/examples && make install)
