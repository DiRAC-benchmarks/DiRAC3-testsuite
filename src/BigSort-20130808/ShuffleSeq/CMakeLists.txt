cmake_minimum_required(VERSION 2.8)
project (ShuffSeq)

if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif (NOT DEFINED BUILD_SHARED_LIBS)


set (CMAKE_C_FLAGS "-homp -hrestrict=a -O3 -hfp3")
set (CMAKE_CXX_FLAGS "-homp -hrestrict=a -O3 -hfp3")


file(GLOB ShuffSeq_SOURCES "*.cpp")

add_executable(ShuffSeq ${ShuffSeq_SOURCES})
