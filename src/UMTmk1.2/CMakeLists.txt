# set (CMAKE_Fortran_FLAGS "-homp -O3 -hcpu=x86-64")
set (CMAKE_Fortran_FLAGS "-fopenmp -O3")
file(GLOB UMTmk_SOURCES "*.F90")
add_executable (UMTmk ${UMTmk_SOURCES})
