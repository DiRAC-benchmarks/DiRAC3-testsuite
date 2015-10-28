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


add_executable (bsmbench_comms ${SRCS})
target_include_directories(bsmbench_comms PUBLIC ${TOPDIR}/Include_comms)
target_link_libraries(bsmbench_comms ${MPI_C_LIBRARIES} ${M_LIBRARIES})



add_executable (bsmbench_balance ${SRCS})
target_include_directories(bsmbench_balance PUBLIC ${TOPDIR}/Include_balanced)
target_link_libraries(bsmbench_balance ${MPI_C_LIBRARIES} ${M_LIBRARIES})


add_executable (bsmbench_compute ${SRCS})
target_include_directories(bsmbench_compute PUBLIC ${TOPDIR}/Include_compute)
target_link_libraries(bsmbench_compute ${MPI_C_LIBRARIES} ${M_LIBRARIES})
