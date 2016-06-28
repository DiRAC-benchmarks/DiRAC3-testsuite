# Swift Benchmark
ExternalProject_Add(swiftsim-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/swiftsim-benchmark.git
  GIT_TAG benchmark
  SOURCE_DIR ${CMAKE_BINARY_DIR}/swiftsim-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND libtoolize
    && ./autogen.sh
    && CC=${CMAKE_C_COMPILER} ./configure --prefix=${CMAKE_SOURCE_DIR} --with-metis
  BUILD_COMMAND make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/swiftsim-benchmark/examples && make install)

# Download CosmoVolume initial conditions
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/download_ICs.cmake
  "File(DOWNLOAD http://icc.dur.ac.uk/~jlvc76/Files/SWIFT/cosmoVolume.hdf5 ${DIRAC3_TESTSUITE_INPUTS_PATH}/cosmoVolume.hdf5)")
ExternalProject_Add_Step(swiftsim-benchmark download_ICs
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/download_ICs.cmake)

# Create run and submission scripts for Swift jobs
CREATE_DIRAC3_JOB(BENCHMARK_NAME "swift" BENCHMARK_TAG "single" JOB_NODES  1)
CREATE_DIRAC3_JOB(BENCHMARK_NAME "swift" BENCHMARK_TAG "multi"  JOB_NODES 16)
