cmake_minimum_required(VERSION 2.8)
project (GenSeq)



find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif (NOT DEFINED BUILD_SHARED_LIBS)


set (CMAKE_C_FLAGS "-openmp  -O3 ")
set (CMAKE_CXX_FLAGS "-openmp  -O3 ")


file(GLOB GenSeq_SOURCES "*.cpp")

add_executable(GenSeq ${GenSeq_SOURCES})





target_link_libraries(GenSeq ${MPI_LIBRARIES})




