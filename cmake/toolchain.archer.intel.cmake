# Compilers
set(CMAKE_C_COMPILER       /opt/cray/craype/2.4.2/bin/cc)
set(CMAKE_CXX_COMPILER     /opt/cray/craype/2.4.2/bin/CC)
set(CMAKE_Fortran_COMPILER /opt/cray/craype/2.4.2/bin/ftn)

# Hardware Specification
set(DIRAC3_HOST          archer)
set(HOST_CPUS_PER_NODE       24)
set(HOST_THREADS_PER_CPU      2)
set(HOST_COMPILER_ID      intel)
set(HOST_SUBMIT_COMMAND    qsub)

# Speciy the path to the FFTW2 library manually instead
# of using the module due to a conflict error on Archer
set(FFTW2_ROOT /opt/cray/fftw/2.1.5.9)

# Find MPI library but use aprun for parallel jobs
find_program(MPIEXEC aprun)
find_package(MPI REQUIRED)
set(MPIEXEC_NUMPROC_FLAG -n)
set(MPIEXEC_PREFLAGS "-d $OMP_NUM_THREADS -cc numa_node")
