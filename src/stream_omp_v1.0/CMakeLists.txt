set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3")
add_executable(stream_omp_c stream_omp_v1.0.c)

# Script defining modules required by stream
set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.stream.${DIRAC3_HOST}")

# Run and submission scripts for stream
set(JOB_NAME       "Stream")
set(SCRIPT_RUN     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.stream.${DIRAC3_HOST}")
set(SCRIPT_RUN_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/run.stream.in")
set(SCRIPT_SUB     "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/submit.stream.${DIRAC3_HOST}")
set(SCRIPT_SUB_IN  "${DIRAC3_TESTSUITE_TEMPLATES_PATH}/submit.${DIRAC3_HOST}.in")
configure_file(${SCRIPT_RUN_IN} ${SCRIPT_RUN})
configure_file(${SCRIPT_SUB_IN} ${SCRIPT_SUB})
file(GENERATE OUTPUT ${SCRIPT_RUN} INPUT ${SCRIPT_RUN})
file(GENERATE OUTPUT ${SCRIPT_SUB} INPUT ${SCRIPT_SUB})
