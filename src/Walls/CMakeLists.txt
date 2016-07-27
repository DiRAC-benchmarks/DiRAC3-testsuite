# Build Walls Benchmark
ExternalProject_Add(walls-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/Walls-benchmark.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/walls-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND mkdir -p bin build
    && cd build
    && CC=${CMAKE_C_COMPILER} ${CMAKE_COMMAND} ${CMAKE_BINARY_DIR}/walls-benchmark -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR} -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/walls-benchmark/build && make && cp ${CMAKE_BINARY_DIR}/walls-benchmark/inputs/dados.txt ${DIRAC3_TESTSUITE_INPUTS_PATH}
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/walls-benchmark/build && make install)

# Create run and submission scripts for Walls jobs
set(WALLS_INPUT_FILE ${DIRAC3_TESTSUITE_INPUTS_PATH}/dados.txt)
set(WALLS_MPI_LAYOUT "1.1.1")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "walls.cache.3d" JOB_NODES  1)
set(WALLS_MPI_LAYOUT "2.2.2")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "walls.grid.3d"  JOB_NODES  8)
set(WALLS_MPI_LAYOUT "1.1.1.1")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "walls.cache.4d" JOB_NODES  1)
set(WALLS_MPI_LAYOUT "2.2.2.2")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "walls.grid.4d"  JOB_NODES 16)
