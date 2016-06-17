# QPhiX Benchmark
ExternalProject_Add(qphix-benchmark
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
  GIT_REPOSITORY git@github.com:DiRAC-benchmarks/qphix.git
  GIT_TAG devel
  SOURCE_DIR ${CMAKE_BINARY_DIR}/qphix-benchmark
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND autoreconf &&
    CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} ./configure
    "CXXFLAGS=${OpenMP_CXX_FLAGS} -O3 -finline-functions -fno-alias -std=c++11 -xAVX -restrict"
    "CFLAGS=${OpenMP_C_FLAGS} -O3 -fno-alias -std=c99 -xAVX -restrict"
    --enable-parallel-arch=scalar --enable-soalen=8 --enable-proc=AVX --enable-openmp
    --prefix=${CMAKE_SOURCE_DIR}
  BUILD_COMMAND make
  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/qphix-benchmark/tests && make install)
