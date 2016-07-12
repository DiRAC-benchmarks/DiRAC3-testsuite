# Compilers
set(CMAKE_C_COMPILER       /opt/sgi/mpt/mpt-2.13/bin/mpicc)
set(CMAKE_CXX_COMPILER     /opt/sgi/mpt/mpt-2.13/bin/mpicxx)
set(CMAKE_Fortran_COMPILER /opt/sgi/mpt/mpt-2.13/bin/mpif90)

# Hardware Specification
set(DIRAC3_HOST          cosmos)
set(HOST_CPUS_PER_NODE        8)
set(HOST_THREADS_PER_CPU      1)
set(HOST_COMPILER_ID      clang)
set(HOST_SUBMIT_COMMAND    msub)

# OpenMP flags (CMake cannot detect itself)
set(OPENMP_FOUND True)
set(OpenMP_C_FLAGS   "-fopenmp=libomp")
set(OpenMP_CXX_FLAGS "-fopenmp=libomp")

set(FFTW2_ROOT $ENV{FFTW2_ROOT})
