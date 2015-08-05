set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3")
add_executable(stream_omp_v1 stream_omp_v1.0.c)
