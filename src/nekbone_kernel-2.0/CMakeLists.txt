cmake_minimum_required (VERSION 2.6)
project (nekbonemk)
enable_language (Fortran)

set(CMAKE_Fortran_COMPILER "ftn")

set (CMAKE_Fortran_FLAGS "-homp -O3 -hcpu=x86-64")

file(GLOB nekbone_SOURCES "./src/*.f")

add_executable (nekbonemk ${nekbone_SOURCES})


