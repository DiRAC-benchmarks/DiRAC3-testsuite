set (CMAKE_C_FLAGS "${OpenMP_C_FLAGS} -std=c99 -DGRAPH_GENERATOR_MPI")
include_directories(${MPI_C_INCLUDE_PATH})

file(GLOB GRAPH500_MPI_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mpi/*.c")
file(GLOB GRAPH500_MPI_BFS_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs*.c")
list(REMOVE_ITEM GRAPH500_MPI_C_SOURCES ${GRAPH500_MPI_BFS_SOURCES})
set(GRAPH500_MPI_GENERATOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/generator/graph_generator.c" "${CMAKE_CURRENT_SOURCE_DIR}/generator/make_graph.c" "${CMAKE_CURRENT_SOURCE_DIR}/generator/splittable_mrg.c" "${CMAKE_CURRENT_SOURCE_DIR}/generator/utils.c")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} "${CMAKE_CURRENT_SOURCE_DIR}/generator")

add_executable(graph500_mpi_simple "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs_simple.c" ${GRAPH500_MPI_C_SOURCES} ${GRAPH500_MPI_GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_simple ${MPI_C_LIBRARIES} ${M_LIBRARIES})

add_executable(graph500_mpi_one_sided "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs_one_sided.c" ${GRAPH500_MPI_C_SOURCES} ${GRAPH500_MPI_GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_one_sided ${MPI_C_LIBRARIES} ${M_LIBRARIES})

add_executable(graph500_mpi_replicated "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs_replicated.c" ${GRAPH500_MPI_C_SOURCES} ${GRAPH500_MPI_GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_replicated ${MPI_C_LIBRARIES} ${M_LIBRARIES})

add_executable(graph500_mpi_replicated_csc "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs_replicated_csc.c" ${GRAPH500_MPI_C_SOURCES} ${GRAPH500_MPI_GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_replicated_csc ${MPI_C_LIBRARIES} ${M_LIBRARIES})

add_executable(graph500_mpi_custom "${CMAKE_CURRENT_SOURCE_DIR}/mpi/bfs_custom.c" ${GRAPH500_MPI_C_SOURCES} ${GRAPH500_MPI_GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_custom ${MPI_C_LIBRARIES} ${M_LIBRARIES})
