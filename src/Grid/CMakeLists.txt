# Build Grid Benchmark
ExternalProject_Add(grid-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:paboyle/Grid
  GIT_TAG develop
  SOURCE_DIR ${CMAKE_BINARY_DIR}/grid-benchmark
  BUILD_IN_SOURCE 1
	CONFIGURE_COMMAND aclocal -I m4
    && autoheader -f
    && automake -f --add-missing
    && autoconf -f
		&& sed -i "s/prerequisites lib benchmarks tests/prerequisites lib benchmarks/g" Makefile.am
		&& mkdir -p builds/dirac3testsuite
		&& cd builds/dirac3testsuite
		&& CXX=${CMAKE_CXX_COMPILER} CC=${CMAKE_C_COMPILER} ../../configure --enable-precision=single --enable-simd=AVX "CXXFLAGS=${OpenMP_CXX_FLAGS} -mavx -O3 -std=c++11" "LIBS=${OpenMP_CXX_FLAGS} -lgmp -lmpfr" --enable-comms=mpi --prefix=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/dirac3testsuite && make all
	INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/grid-benchmark/builds/dirac3testsuite && make install)

# Create run and submission scripts for Grid jobs
set(GRID_MPI_LAYOUT "1.1.1.1")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "grid" BENCHMARK_TAG "single" JOB_NODES  1)
set(GRID_MPI_LAYOUT "2.2.2.2")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "grid" BENCHMARK_TAG "multi"  JOB_NODES 16)
