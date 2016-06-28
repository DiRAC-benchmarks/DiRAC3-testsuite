# QMP Library
ExternalProject_Add(qmp-library
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/qmp.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/qmp-library
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND autoreconf &&
    CC=${CMAKE_C_COMPILER} ./configure "CFLAGS=-std=c99"
    --with-qmp-comms-type=MPI --prefix=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND make
  INSTALL_COMMAND make install)

# QPhiX Benchmark
ExternalProject_Add(qphix-benchmark
  DEPENDS qmp-library
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/qphix.git
  GIT_TAG devel
  SOURCE_DIR ${CMAKE_BINARY_DIR}/qphix-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND autoreconf &&
    CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ./configure
    "CXXFLAGS=${OpenMP_CXX_FLAGS} -O3 -finline-functions -fno-alias -std=c++11 -xAVX -restrict"
    "CFLAGS=${OpenMP_C_FLAGS} -O3 -fno-alias -std=c99 -xAVX -restrict"
    --enable-parallel-arch=parscalar --enable-soalen=8 --enable-proc=AVX --enable-openmp
    --prefix=${CMAKE_SOURCE_DIR} --with-qmp=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/qphix-benchmark/tests && make install)

# Create run and submission scripts for QPhiX
set(QPHIX_MPI_LAYOUT "1 1 1 1")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "qphix" BENCHMARK_TAG "single" JOB_NODES  1)
set(QPHIX_MPI_LAYOUT "2 2 2 2")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "qphix" BENCHMARK_TAG "multi"  JOB_NODES 16)
