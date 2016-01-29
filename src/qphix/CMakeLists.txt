#CMAKE build version to integrate this code into the DiRAC repo
#replicates the work done in the original QPHIX build system - autotools

#We set the variables in CMakeLists.txt instead of parsing arguments at this stage
#Contact DiRAC Benchmark team for any further questions


cmake_minimum_required (VERSION 2.6)
project(qphix C CXX)
#AC_CONFIG_AUX_DIR(config)

SET(PARALLEL_ARCH "scalar")

SET(SOALEN 8)

SET(USE_CEAN "no")

SET(USE_MM_MALLOC "no")

SET(build_clover "no")

SET(USE_FAKE_COMMS "no")

SET(qmt_enabled "no")

SET(QDPXX_GIVEN "no")

SET(proc "AVX")

set(QMP_HOME "/cosma5/data/dr002/dc-kash1/qmp/build")

MESSAGE("DEBUG ${QMP_HOME}")


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -finline-functions -std=c++11 -mavx -restrict")
set(CMAKE_C_CFLAGS "${CMAKE_C_CFLAGS}  -g  -O3 -std=c99 -mavx -restrict")

FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    MESSAGE("DEBUG: adding this ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    MESSAGE("DEBUG: adding this for cxx ${OpenMP_CXX_FLAGS}")
  ENDIF()


if(${QDPXX_GIVEN} STREQUAL "yes")
  MESSAGE("this will never execute")
else()
  if(${PARALLEL_ARCH} STREQUAL "scalar")
    MESSAGE("Scalar Build! Not checking for QMP")
    SET(QPHIX_SCALAR 1)
    set(QPHIX_AUX_CFLAGS "")
		set(QPHIX_AUX_LDFLAGS "")
		set(QPHIX_AUX_LIBS "")
    set(QPHIX_BUILD_WITH_QDP false)
    SET(QPHIX_PARSCALAR false)
  elseif(${PARALLEL_ARCH} STREQUAL "parscalar")
	MESSAGE("Parscalar build! Checking for QMP")
	if("X${QMP_HOME}X" STREQUAL "XX")
		FIND_PROGRAM(QMP_CONFIG qmp-config PATHS ENV PATH  NO_DEFAULT_PATH)
	else()
		FIND_PROGRAM(QMP_CONFIG qmp-config PATHS ${QMP_HOME}/bin ENV PATH  NO_DEFAULT_PATH)
		MESSAGE(" QMP_CONFIG SERACH ${QMP_HOME}/bin")

	endif()
	
	if("X${QMP_CONFIG}X" STREQUAL "XX")
		MESSAGE( WARNING "configuration program qmp-config not found")
		MESSAGE( WARNING "Set environment variables QMP_CFLAGS QMP_LDFAGS QMP_LIBS before configure")
	else()
		MESSAGE("Found QMP configuration program ${QMP_CONFIG}")
		#set (EXECUTE_COMMAND ${QMP_CONFIG} --cflags)
		#execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE QPHIX_AUX_CFLAGS)
		#set(${CMAKE_C_FLAGS} "${CMAKE_C_FLAGS} ${QPHIX_AUX_CFLAGS}")

		#set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")
		#set (EXECUTE_COMMAND ${QMP_CONFIG} --ldflags)
		#execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE QPHIX_AUX_LDFLAGS)
		
		FIND_LIBRARY(QMP_LIB qmp "${QMP_HOME}/lib")
		MESSAGE("DEBUG ${QMP_LIB} found")
		
		#set (EXECUTE_COMMAND ${QMP_CONFIG}  --libs)
		#execute_process(COMMAND ${EXECUTE_COMMAND} RESULT_VARIABLE QPHIX_AUX_LIBS)
	endif() #qmpconfig
	#TODO Sid to include simple program check here to test the QMP flags

  endif() #parallel arch
endif() #qdpxx


if(${PARALLEL_ARCH} STREQUAL "scalar")
  add_definitions(-DQPHIX_SCALAR=1)
  if(${USE_FAKE_COMMS} STREQUAL "yes")
    MESSAGE("Going to use Fake comms ")
    add_definitions(-DQPHIX_DO_COMMS=1)
    add_definitions(-DQPHIX_FAKE_COMMS=1)
  else()
    MESSAGE("This version does no comms")
  endif()

elseif(${PARALLEL_ARCH} STREQUAL "parscalar")

	add_definitions(-DQPHIX_PARSCALAR=1)
	if(${USE_FAKE_COMMS} STREQUAL "yes")
		MESSAGE(SEND_ERROR "Cannot use parscalar arch with fake comms")
	else()
		MESSAGE("Regular QMP COMMS")
		add_definitions(-DQPHIX_DO_COMMS=1)
		add_definitions(-DQPHIX_QMP_COMMS=1)
	endif()
endif()

if(${SOALEN} EQUAL 1)
  MESSAGE("Building for SOALEN=1")
  add_definitions(-DQPHIX_SOALEN=1)
elseif(${SOALEN} EQUAL 4)
  MESSAGE("Building for SOALEN=4")
  add_definitions(-DQPHIX_SOALEN=4)
elseif(${SOALEN} EQUAL 8)
  MESSAGE("Building for SOALEN=8")
  add_definitions(-DQPHIX_SOALEN=8)
elseif(${SOALEN} EQUAL 16)
  MESSAGE("Building for SOALEN=16")
  add_definitions(-DQPHIX_SOALEN=16)
else()
  MESSAGE(SEND_ERROR "Unsupported SOALEN")
endif()


if(${USE_CEAN} STREQUAL "yes")
  MESSAGE("Enabling Cilk Notation Use")
  add_definitions(-DQPHIX_USE_CEAN=1)
endif()

if(${USE_MM_MALLOC} STREQUAL "yes")
  MESSAGE("Enabling _mm_malloc use")
  add_definitions(-DQPHIX_USE_MM_MALLOC=1)
elseif()
  MESSAGE("Not using _mm_malloc. Using posix memalign")
endif()

if(${build_clover} STREQUAL "yes")
  MESSAGE("Enabling Clover Term")
  add_definitions(-DQPHIX_BUILD_CLOVER=1)
else()
  MESSAGE("Not Building Clover Term")
endif()

SET(build_qpx "no")

if(${proc} STREQUAL "AVX" OR ${proc} STREQUAL "avx")
  add_definitions(-DQPHIX_AVX_SOURCE=1)
  add_definitions(-DQPHIX_LLC_CACHE_ALIGN=64)
  MESSAGE("Configuring AVX Specializations")
  if(${SOALEN} GREATER 8)
    MESSAGE(SEND_ERROR "Cannot have SOALEN more than 8 for AVX")
  endif()

elseif(${proc} STREQUAL "mic" OR ${proc} STREQUAL "MIC")
  add_definitions(-DQPHIX_MIC_SOURCE=1)
  add_definitions(-DQPHIX_LLC_CACHE_ALIGN=64)
  MESSAGE("Configuring MIC Specializations")

elseif(${proc} STREQUAL "SCALAR" OR ${proc} STREQUAL "scalar")
  add_definitions(-DQPHIX_SCALAR_SOURCE=1)
  add_definitions(-DQPHIX_LLC_CACHE_ALIGN=64)
  MESSAGE("Configuring Scalar Specializations")

elseif(${proc} STREQUAL "QPX" OR ${proc} STREQUAL "qpx")
  add_definitions(-DQPHIX_QPX_SOURCE=1)
  add_definitions(-DQPHIX_LLC_CACHE_ALIGN=128)
  MESSAGE("Configuring QPX Specializations ")
  if(NOT ${SOALEN} EQUAL 4 )
    MESSAGE("Cannot have SOALEN not EQUAL to 4 for QPX")
  endif()
  set(build_qpx "yes")
else()
  MESSAGE("Not generating any code specializations")
endif()

if(${build_clover} STREQUAL "yes" )
  set(QPHIX_BUILD_CLOVER true)
endif()

if(${build_qpx} STREQUAL "yes")
  SET(QPHIX_BUILD_QPX true)
endif()

MESSAGE("DEBUG: include dir : ${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${QMP_HOME}/include)


find_package(MPI)



add_subdirectory(lib)
#add_subdirectory(include)
add_subdirectory(tests)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/qphix/qphix_config_internal.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/include/qphix/qphix_config_internal.h) 



add_executable(time_dslash_noqdp  ./tests/time_dslash_noqdp.cc ./tests/timeDslashNoQDP.h   ./tests/timeDslashNoQDP.cc)
MESSAGE("qmp link lib ${QMP_LIB}")
#add_library(LIBQMP ${QMP_LIB})
target_link_libraries(time_dslash_noqdp qphix_solver ${QMP_LIB} ${MPI_C_LIBRARIES})

# Generate scripts for DiRAC3 benchmarking
if(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)
  # Script defining modules required by QPhiX
  set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.qphix.${DIRAC3_HOST}")

  # Run and submission scripts for QPhiX
  set(JOB_NAME       "QPhiX")
  set(SCRIPT_RUN     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.qphix.${DIRAC3_HOST}")
  set(SCRIPT_RUN_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.qphix.in")
  set(SCRIPT_SUB     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.qphix.${DIRAC3_HOST}")
  set(SCRIPT_SUB_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
  configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
  configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
  file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
  file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})
endif(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)
