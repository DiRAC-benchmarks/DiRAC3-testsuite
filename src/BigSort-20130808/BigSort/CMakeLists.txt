cmake_minimum_required(VERSION 2.8)
project (BigSort)


find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})



if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif (NOT DEFINED BUILD_SHARED_LIBS)


set (CMAKE_C_FLAGS "-openmp -O3")
set (CMAKE_CXX_FLAGS "-openmp -O3")


file(GLOB BigSort_SOURCES "*.cpp")

add_executable(BigSort ${BigSort_SOURCES})

target_link_libraries(BigSort ${MPI_LIBRARIES})
