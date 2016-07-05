# MPI Library
include_directories(${MPI_C_INCLUDE_PATH})

# Source Headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Source Files
file(GLOB IOR_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Remove HDF and NetCDF
# :TODO: This should be configured separately alongside config.h
file(GLOB IOR_SOURCE_REMOVE ${CMAKE_CURRENT_SOURCE_DIR}/aiori-NCMPI.c ${CMAKE_CURRENT_SOURCE_DIR}/aiori-HDF5.c)
list(REMOVE_ITEM IOR_SOURCE ${IOR_SOURCE_REMOVE})

# IOR Executable
add_executable(ior ${IOR_SOURCE})
target_link_libraries(ior ${MPI_C_LIBRARIES})

# Preprocessor Definitions
add_definitions(-DHAVE_CONFIG_H)

# Generate scripts for DiRAC3 benchmarks
# Run and submission scripts for each IOR configuration
foreach(IOR_CONFIGURATION "NS.FPS" "NS.SHF" "NW.FPS" "NW.SHF")
  set(IOR_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/inputs/${IOR_CONFIGURATION}")
  CREATE_DIRAC3_JOB(BENCHMARK_NAME "ior" BENCHMARK_TAG ${IOR_CONFIGURATION} JOB_NODES 1)
endforeach(IOR_CONFIGURATION)
