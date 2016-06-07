# Grid Benchmark
ExternalProject_Add(grid-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:paboyle/Grid
  GIT_TAG master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/grid-benchmark
  BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND ./scripts/reconfigure_script
		&& sed -i "s/lib tests benchmarks/lib benchmarks/g" Makefile.am
		&& mkdir -p builds/dirac3testsuite
		&& cd builds/dirac3testsuite
		&& CXX=${CMAKE_CXX_COMPILER} ../../configure --enable-precision=single --enable-simd=AVX "CXXFLAGS=-mavx -fopenmp -O3 -std=c++11" "LIBS=-fopenmp -lgmp -lmpfr" --enable-comms=mpi --prefix=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/dirac3testsuite && make all
	INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/dirac3testsuite && make install)

if(DIRAC3_TESTSUITE_MODULES_PATH AND DIRAC3_TESTSUITE_SCRIPTS_PATH AND DIRAC3_TESTSUITE_TEMPLATES_PATH AND DIRAC3_HOST)

	# Script defining modules required by Grid
	set(SCRIPT_MODULES "${DIRAC3_TESTSUITE_MODULES_PATH}/modules.grid.${DIRAC3_HOST}")

	# Run and submission scripts for GridSingleNode
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
