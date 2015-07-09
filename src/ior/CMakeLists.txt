# MPI Library
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
# C Math Library
find_library(M_LIBRARY m REQUIRED)

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
target_link_libraries(ior ${MPI_LIBRARIES} ${M_LIBRARY})

# Preprocessor Definitions
add_definitions(-DHAVE_CONFIG_H)
