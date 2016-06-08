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
if(NOT DIRAC3_HOST)
	set(DIRAC3_HOST "default")
endif(NOT DIRAC3_HOST)

# Submission script template and module file for the system
set(DIRAC3_SUB_SCRIPT_IN "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
set(DIRAC3_MODULE_FILE   "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.${DIRAC3_HOST}")

# Jobscript Generation Function
function(CREATE_DIRAC3_JOB BENCHMARK_NAME)
if(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)
	if(ARGV1)
		set(DIRAC3_JOB_NAME "${BENCHMARK_NAME}.${ARGV1}")
	else()
		set(DIRAC3_JOB_NAME "${BENCHMARK_NAME}")
	endif(ARGV1)
  set(DIRAC3_RUN_SCRIPT     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.${DIRAC3_JOB_NAME}.${DIRAC3_HOST}")
  set(DIRAC3_RUN_SCRIPT_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.${BENCHMARK_NAME}.in")
  set(DIRAC3_SUB_SCRIPT     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.${DIRAC3_JOB_NAME}.${DIRAC3_HOST}")
  configure_file(${DIRAC3_RUN_SCRIPT_IN} ${DIRAC3_RUN_SCRIPT})
  configure_file(${DIRAC3_SUB_SCRIPT_IN} ${DIRAC3_SUB_SCRIPT})
  file(GENERATE OUTPUT ${DIRAC3_RUN_SCRIPT} INPUT ${DIRAC3_RUN_SCRIPT})
  file(GENERATE OUTPUT ${DIRAC3_SUB_SCRIPT} INPUT ${DIRAC3_SUB_SCRIPT})
endif()
endfunction(CREATE_DIRAC3_JOB)

# Add source
add_subdirectory(src)
