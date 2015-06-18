# Find OpenMP and MPI
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Compiler flags
# set(MILCMK_COPT "-O5 -qhot=level=2 -qsimd=auto")
set(MILCMK_COPT "-std=c99 -O3")
set(MILCMK_CFLAGSA "-DHAVE_XLC")
set(MILCMK_CFLAGSF "${MILCMK_CFLAGSA} ${MILCMK_COPT} ${OpenMP_C_FLAGS} -DQLA_Precision=\\'F\\' -DQLA_Colors=3 -DQLA_Nc=3")
set(MILCMK_CFLAGSD "${MILCMK_CFLAGSA} ${MILCMK_COPT} ${OpenMP_C_FLAGS} -DQLA_Precision=\\'D\\' -DQLA_Colors=3 -DQLA_Nc=3")

# Compiler alignment macros
set(MILCMK_ALIGNMENT_SOURCE "alignment_macros.c")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Include directory containing QLA
set(MILCMK_QLA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qla-1.7.1")
include_directories(${MILCMK_QLA_DIR})

# Source files
set(MILCMK_BENCH_SOURCE "qla_bench.c")
FILE(GLOB MILCMK_FSOURCE ${MILCMK_QLA_DIR}/QLA_F3*.c)
FILE(GLOB MILCMK_DSOURCE ${MILCMK_QLA_DIR}/QLA_D3*.c)

# Single precision floating point executable
get_filename_component(MILCMK_FTARGET ${MILCMK_QLA_DIR} NAME_WE)
set(MILCMK_FTARGET "qla_bench-${MILCMK_FTARGET}-f3")
add_executable(${MILCMK_FTARGET} ${MILCMK_BENCH_SOURCE} ${MILCMK_FSOURCE} ${MILCMK_ALIGNMENT_SOURCE})
set_target_properties(${MILCMK_FTARGET} PROPERTIES COMPILE_FLAGS "${MILCMK_CFLAGSF}")
target_link_libraries(${MILCMK_FTARGET} ${OpenMP_C_FLAGS} ${MPI_LIBRARIES})

# Double precision floating point executable
get_filename_component(MILCMK_DTARGET ${MILCMK_QLA_DIR} NAME_WE)
set(MILCMK_DTARGET "qla_bench-${MILCMK_DTARGET}-d3")
add_executable(${MILCMK_DTARGET} ${MILCMK_BENCH_SOURCE} ${MILCMK_DSOURCE} ${MILCMK_ALIGNMENT_SOURCE})
set_target_properties(${MILCMK_DTARGET} PROPERTIES COMPILE_FLAGS "${MILCMK_CFLAGSD}")
target_link_libraries(${MILCMK_DTARGET} ${OpenMP_C_FLAGS} ${MPI_LIBRARIES})
