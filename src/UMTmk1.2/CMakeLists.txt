cmake_minimum_required (VERSION 2.6)
project (UMTmk)
enable_language (Fortran)

set(CMAKE_Fortran_COMPILER "ftn")

set (CMAKE_Fortran_FLAGS "-homp -O3 -hcpu=x86-64")

file(GLOB UMTmk_SOURCES "*.F90")

add_executable (UMTmk ${UMTmk_SOURCES})


