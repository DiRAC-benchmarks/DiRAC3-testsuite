#CMAKE build version to integrate this code into the DiRAC repo
#replicates the work done in the original BSM Bench build system - make

#We set the variables in CMakeLists.txt instead of parsing arguments at this stage
#Contact DiRAC Benchmark team for any further questions

cmake_minimum_required (VERSION 2.6)
project(BSMBench C CXX)

macro (add_sources)
    file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
    foreach (_src ${ARGN})
        if (_relPath)
            list (APPEND SRCS "${_relPath}/${_src}")
        else()
            list (APPEND SRCS "${_src}")
        endif()
    endforeach()
    if (_relPath)
        # propagate SRCS to parent directory
        set (SRCS ${SRCS} PARENT_SCOPE)
    endif()
endmacro()


set(TOPDIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(MKDIR ${TOPDIR}/Make)


FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()

find_package(MPI)
find_library(M_LIBRARY m)

SET(QDPXX_GIVEN "no")

SET(proc "AVX")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -finline-functions -std=c++11 -mavx  -Drestrict=__restrict__ ")
set(CMAKE_C_CFLAGS "${CMAKE_C_CFLAGS}  -g  -O3 -std=c99 -mavx -Drestrict=__restrict__ ")

set(REPR "REPR_FUNDAMENTAL")

# add_definitions(-DREPR_NAME=${REPR})

add_definitions(-DREPR_NAME="$REPR")
add_definitions("-DWITH_MPI")
add_definitions("-D${REPR}")

FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()

  set(NG 3)
  MESSAGE("Writing suN headers...")
 # file(COPY ${TOPDIR}/Include/su${NG}.h DESTINATION ${TOPDIR}/Include/suN.h)
 # file(COPY ${TOPDIR}/Include/su${NG}_types.h DESTINATION ${TOPDIR}/Include/suN_types.h)
  MESSAGE("Writing representation header...")
 # file(COPY ${TOPDIR}/Include/su$(NG)_repr_func.h ${TOPDIR}/Include/suN_repr_func.h)



add_subdirectory(Include)
add_subdirectory(Geometry)
add_subdirectory(IO)
add_subdirectory(Memory)
add_subdirectory(Observables)
add_subdirectory(Random)
add_subdirectory(Update)
add_subdirectory(Utils)
add_subdirectory(Error)
add_subdirectory(Inverters)
add_subdirectory(Make)
add_subdirectory(Bench)

message(STATUS "${SRCS}")
include_directories(${TOPDIR}/Include)
add_executable (bsmbench ${SRCS})
target_link_libraries(bsmbench  ${MPI_C_LIBRARIES} ${M_LIBRARIES})
