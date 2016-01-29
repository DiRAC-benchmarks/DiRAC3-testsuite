# Include MPI
include_directories(${MPI_C_INCLUDE_PATH})

# Compiler flags
# set(MILCMK_COPT "-O5 -qhot=level=2 -qsimd=auto")
set(MILCMK_COPT "-std=c99 -O3")
set(MILCMK_CFLAGSA "-DHAVE_XLC")
set(MILCMK_CFLAGSF "${MILCMK_CFLAGSA} ${MILCMK_COPT} ${OpenMP_C_FLAGS} -DQLA_Precision=\\'F\\' -DQLA_Colors=3 -DQLA_Nc=3")
set(MILCMK_CFLAGSD "${MILCMK_CFLAGSA} ${MILCMK_COPT} ${OpenMP_C_FLAGS} -DQLA_Precision=\\'D\\' -DQLA_Colors=3 -DQLA_Nc=3")

# Compiler alignment macros
set(MILCMK_ALIGNMENT_SOURCE "alignment_macros.c")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Include directory containing QLA
set(MILCMK_QLA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qla-1.7.1")
include_directories(${MILCMK_QLA_DIR})

# Source files
set(MILCMK_BENCH_SOURCE "qla_bench.c")
file(GLOB MILCMK_FSOURCE ${MILCMK_QLA_DIR}/QLA_F3*.c)
file(GLOB MILCMK_DSOURCE ${MILCMK_QLA_DIR}/QLA_D3*.c)

# Single precision floating point executable
get_filename_component(MILCMK_FTARGET ${MILCMK_QLA_DIR} NAME_WE)
set(MILCMK_FTARGET "qla_bench-${MILCMK_FTARGET}-f3")
add_executable(${MILCMK_FTARGET} ${MILCMK_BENCH_SOURCE} ${MILCMK_FSOURCE} ${MILCMK_ALIGNMENT_SOURCE})
set_target_properties(${MILCMK_FTARGET} PROPERTIES COMPILE_FLAGS "${MILCMK_CFLAGSF}")
target_link_libraries(${MILCMK_FTARGET} ${OpenMP_C_FLAGS} ${MPI_C_LIBRARIES} ${M_LIBRARIES})

# Double precision floating point executable
get_filename_component(MILCMK_DTARGET ${MILCMK_QLA_DIR} NAME_WE)
set(MILCMK_DTARGET "qla_bench-${MILCMK_DTARGET}-d3")
add_executable(${MILCMK_DTARGET} ${MILCMK_BENCH_SOURCE} ${MILCMK_DSOURCE} ${MILCMK_ALIGNMENT_SOURCE})
set_target_properties(${MILCMK_DTARGET} PROPERTIES COMPILE_FLAGS "${MILCMK_CFLAGSD}")
target_link_libraries(${MILCMK_DTARGET} ${OpenMP_C_FLAGS} ${MPI_C_LIBRARIES} ${M_LIBRARIES})

# Script defining modules required by MILC
set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.milc.${DIRAC3_HOST}")

# Run and submission scripts for MILC-F3
set(JOB_NAME       "MILC-F3")
set(SCRIPT_RUN     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.milcf3.${DIRAC3_HOST}")
set(SCRIPT_RUN_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.milcf3.in")
set(SCRIPT_SUB     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.milcf3.${DIRAC3_HOST}")
set(SCRIPT_SUB_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})

# Run and submission scripts for MILC-D3
set(JOB_NAME       "MILC-D3")
set(SCRIPT_RUN     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.milcd3.${DIRAC3_HOST}")
set(SCRIPT_RUN_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.milcd3.in")
set(SCRIPT_SUB     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.milcd3.${DIRAC3_HOST}")
set(SCRIPT_SUB_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})
