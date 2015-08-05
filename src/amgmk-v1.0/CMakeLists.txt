file(GLOB AMGMK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
# set (CMAKE_C_FLAGS "-homp -O3 -hcpu=x86-64")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3")
add_executable (AMGmk ${AMGMK_SOURCES})
