# Compilers
set(CMAKE_C_COMPILER       /cosma/local/platform_mpi/9.1.2/bin/mpicc)
set(CMAKE_CXX_COMPILER     /cosma/local/platform_mpi/9.1.2/bin/mpic++)
set(CMAKE_Fortran_COMPILER /cosma/local/platform_mpi/9.1.2/bin/mpif90)

# Hardware Specification
set(DIRAC3_HOST          cosma)
set(HOST_CPUS_PER_NODE      16)
set(HOST_THREADS_PER_CPU     2)

# Find MPI library but use aprun for parallel jobs
find_package(MPI REQUIRED)
set(MPIEXEC_NUMPROC_FLAG -n)
