# Compilers
set(CMAKE_C_COMPILER       /shared/ucl/apps/intel/2018.Update3/compilers_and_libraries_2018.3.222/linux/bin/intel64/icc)
set(CMAKE_CXX_COMPILER     /shared/ucl/apps/intel/2018.Update3/compilers_and_libraries_2018.3.222/linux/bin/intel64/icc)
set(CMAKE_Fortran_COMPILER /shared/ucl/apps/intel/2018.Update3/compilers_and_libraries_2018.3.222/linux/bin/intel64/ifort)

# Hardware Specification
set(DIRAC3_HOST        kathleen)
set(HOST_CPUS_PER_NODE       40)
set(HOST_THREADS_PER_CPU      2)
set(HOST_COMPILER_ID      intel)
set(HOST_SUBMIT_COMMAND    qsub)
