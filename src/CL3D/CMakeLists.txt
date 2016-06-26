#Crown Copyright 2012 AWE.
#
# This file is part of CloverLeaf.
#
# CloverLeaf is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the
# Free Software Foundation, either version 3 of the License, or (at your option)
# any later version.
#
# CloverLeaf is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# CloverLeaf. If not, see http://www.gnu.org/licenses/.

#  @brief Makefile for CloverLeaf
#  @author Wayne Gaudin, Andy Herdman
#  @details Agnostic, platform independent makefile for the Clover Leaf benchmark code.
#  @CMake Build System - DiRAC 3 project team

cmake_minimum_required (VERSION 2.6)
project(Fortran)
enable_language (Fortran)

find_package(MPI REQUIRED)
add_definitions(${MPI_Fortran_COMPILE_FLAGS})
include_directories(${MPI_Fortran_INCLUDE_PATH})
link_directories(${MPI_Fortran_LIBRARIES})

# FFLAGS depend on the compiler
get_filename_component (Fortran_COMPILER_NAME ${CMAKE_Fortran_COMPILER} NAME)


message(${Fortran_COMPILER_NAME})

if (Fortran_COMPILER_NAME MATCHES "gfortran*")
  set (CMAKE_Fortran_FLAGS "-O3 -march=native -funroll-loops")
  set (CMAKE_Fortran_FLAGS_DEBUG   " -O0 -g -O -Wall -Wextra -fbounds-check")
elseif (Fortran_COMPILER_NAME MATCHES "ifort*")
    #set (CMAKE_Fortran_FLAGS "-O3 -no-prec-div -g -xHost -s")
  set (CMAKE_Fortran_FLAGS   "-O0 -g -debug all -check all -traceback -check noarg_temp_created")
elseif (Fortran_COMPILER_NAME MATCHES "ftn.*")
  set (CMAKE_Fortran_FLAGS "-O3 -em  -h acc_model=fast_addr:no_deep_copy:auto_async_all -hlist=m")
  set (CMAKE_Fortran_FLAGS_DEBUG   "-O0 -g -O -Wall -Wextra -fbounds-check")
endif (Fortran_COMPILER_NAME MATCHES "gfortran*")


if (CMAKE_C_COMPILER MATCHES "gcc.*")
  set (CMAKE_C_FLAGS " -O3 -march=native -funroll-loops -mavx")
  set (CMAKE_C_FLAGS_DEBUG   "-O0 -g -em -eD")
elseif (CMAKE_C_COMPILER MATCHES "icc.*")
  set (CMAKE_C_FLAGS " -O3 -no-prec-div -restrict -fno-alias -vec-report=3 -s")
  set (CMAKE_C_FLAGS_DEBUG   " -O0 -g -debug all -traceback")
elseif (CMAKE_C_COMPILER MATCHES "cc.*")
  set (CMAKE_C_FLAGS "-O3 -em -h list=a -hlist=m -hvector3")
  set (CMAKE_C_FLAGS_DEBUG   "-O0 -g -O -Wall -Wextra -fbounds-check")
endif (CMAKE_C_COMPILER MATCHES "gcc.*")

FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    MESSAGE("DEBUG: adding this ${OpenMP_C_FLAGS}")
    SET(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} ${OpenMP_Fortran_FLAGS}")
    SET(CMAKE_Fortran_FLAGS_DEBUG "${CMAKE_Fortran_FLAGS_DEBUG} ${OpenMP_Fortran_FLAGS}")
    MESSAGE("DEBUG: adding this switch ${OpenMP_Fortran_FLAGS}")
ENDIF()

file(GLOB CL3D_F_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.f90)
file(GLOB CL3D_C_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

add_executable(clover_leaf ${CL3D_C_SOURCE} ${CL3D_F_SOURCE})

target_link_libraries(clover_leaf ${MPI_Fortran_LIBRARIES})

# Generate scripts for DiRAC3 benchmarking
set(CL3D_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/inputs/clover.in")
