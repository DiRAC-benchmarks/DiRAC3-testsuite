# Build Gadget Benchmark
ExternalProject_Add(gadget-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/Gadget3EagleBench.git
  GIT_TAG benchmark
  SOURCE_DIR ${CMAKE_BINARY_DIR}/gadget-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND mkdir -p bin build
    && cd build
    && CC=${CMAKE_C_COMPILER} ${CMAKE_COMMAND} ${CMAKE_BINARY_DIR}/gadget-benchmark -DFFTW2_ROOT=${FFTW2_ROOT} -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/gadget-benchmark/build && make P-Gadget3
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/gadget-benchmark/build && make install)

# Create run and submission scripts for Gadget
set(GADGET3_EAGLE_BENCH_RUN_DEFS "${CMAKE_BINARY_DIR}/gadget-benchmark/scripts/L06z000p100.defs")
CREATE_DIRAC3_JOB(BENCHMARK_NAME "gadget" BENCHMARK_TAG "single" JOB_NODES 1)
