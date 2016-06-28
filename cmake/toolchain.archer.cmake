# Compilers
set(CMAKE_C_COMPILER       /opt/cray/craype/2.4.2/bin/cc)
set(CMAKE_CXX_COMPILER     /opt/cray/craype/2.4.2/bin/CC)
set(CMAKE_Fortran_COMPILER /opt/cray/craype/2.4.2/bin/ftn)

# Hardware Specification
set(DIRAC3_HOST          archer)
set(HOST_CPUS_PER_NODE       24)
set(HOST_THREADS_PER_CPU      2)

# Speciy the path to the FFTW2 library manually instead
# of using the module due to a conflict error on Archer
set(FFTW2_ROOT /opt/cray/fftw/2.1.5.9)
