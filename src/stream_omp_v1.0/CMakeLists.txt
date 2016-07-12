set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3")
add_executable(stream_omp_c stream_omp_v1.0.c)

# Run and submission scripts for stream
CREATE_DIRAC3_JOB(BENCHMARK_NAME "stream" JOB_NODES 1)
