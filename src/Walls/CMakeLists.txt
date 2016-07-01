# Build Walls Benchmark
ExternalProject_Add(walls-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/Walls-benchmark.git
  GIT_TAG master
  SOURCE_DIR ${CMAKE_BINARY_DIR}/walls-benchmark
  BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND mkdir -p bin build
                && cd build
                && CC=${CMAKE_C_COMPILER} ${CMAKE_COMMAND} ${CMAKE_BINARY_DIR}/walls-benchmark -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/walls-benchmark/build && make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/walls-benchmark/build && make install)
