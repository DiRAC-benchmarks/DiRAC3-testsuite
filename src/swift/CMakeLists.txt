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
