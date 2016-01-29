# MPI Library
include_directories(${MPI_C_INCLUDE_PATH})

# Source Headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source Files
file(GLOB IOR_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Remove HDF and NetCDF
# :TODO: This should be configured separately alongside config.h
file(GLOB IOR_SOURCE_REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/aiori-NCMPI.c ${CMAKE_CURRENT_SOURCE_DIR}/aiori-HDF5.c)
list(REMOVE_ITEM IOR_SOURCE ${IOR_SOURCE_REMOVE})

# IOR Executable
add_executable(ior ${IOR_SOURCE})
target_link_libraries(ior ${MPI_C_LIBRARIES} ${M_LIBRARIES})

# Preprocessor Definitions
add_definitions(-DHAVE_CONFIG_H)

# Generate scripts for DiRAC3 benchmarks
if(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)

  # Script defining modules required by IOR
  set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.ior.${DIRAC3_HOST}")

  # Run and submission scripts for each IOR configuration
  foreach(IOR_CONFIGURATION "NS.FPS" "NS.SHF" "NW.FPS" "NW.SHF")
    set(IOR_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/inputs/${IOR_CONFIGURATION}")

    set(JOB_NAME      "IOR-NS.FPS")
    set(SCRIPT_RUN    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.ior-${IOR_CONFIGURATION}.${DIRAC3_HOST}")
    set(SCRIPT_RUN_IN "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.ior.in")
    set(SCRIPT_SUB    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.ior-${IOR_CONFIGURATION}.${DIRAC3_HOST}")
    set(SCRIPT_SUB_IN "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
    configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
    configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
    file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
    file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})
  endforeach(IOR_CONFIGURATION)
endif(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)
