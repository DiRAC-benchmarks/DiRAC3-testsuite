# set (CMAKE_Fortran_FLAGS "-homp -O3 -hcpu=x86-64")
set (CMAKE_Fortran_FLAGS "-fopenmp -O3")
file(GLOB nekbone_SOURCES "./src/*.f")
add_executable (nekbonemk ${nekbone_SOURCES})
