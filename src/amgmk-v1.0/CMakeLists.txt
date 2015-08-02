cmake_minimum_required(VERSION 2.8)
project (amgmk)

set (CMAKE_C_FLAGS "-homp -O3 -hcpu=x86-64")

file(GLOB AMGMK_SOURCES "*.c")

add_executable (AMGmk ${AMGMK_SOURCES})
