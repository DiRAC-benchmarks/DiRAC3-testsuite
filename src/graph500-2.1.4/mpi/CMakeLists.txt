cmake_minimum_required(VERSION 2.8)
project (Graph500)

if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif (NOT DEFINED BUILD_SHARED_LIBS)


set (CMAKE_C_FLAGS "-homp -hrestrict=a -O3 -hfp3 -DGRAPH_GENERATOR_MPI")

file(GLOB AMGMK_SOURCES "*.c")

file (GLOB GENERATOR_SOURCES "../generator/graph_generator.c ../generator/make_graph.c ../generator/splittable_mrg.c ../generator/utils.c")
file (GLOB SOURCES "main.c oned_csr.c oned_csc.c utils.c validate.c onesided.c onesided_emul.c")
file (GLOB HEADERS "common.h oned_csr.h oned_csc.h redistribute.h mpi_workarounds.h onesided.h")


add_library(GS "../generator/graph_generator.c ../generator/make_graph.c ../generator/splittable_mrg.c ../generator/utils.c")
add_library(S "main.c oned_csr.c oned_csc.c utils.c validate.c onesided.c onesided_emul.c")
add_library(H "common.h oned_csr.h oned_csc.h redistribute.h mpi_workarounds.h onesided.h")

add_executable(graph500_mpi_simple bfs_simple.c ${SOURCES} ${HEADERS} ${GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_simple ${GS} ${S} ${H})
add_executable(graph500_mpi_one_sided bfs_one_sided.c ${SOURCES} ${HEADERS} ${GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_one_sided ${GS} ${S} ${H})
add_executable(graph500_mpi_replicated bfs_replicated.c ${SOURCES} ${HEADERS} ${GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_replicated ${GS} ${S} ${H})
add_executable(graph500_mpi_replicated_csc bfs_replicated_csc.c ${SOURCES} ${HEADERS} ${GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_replicated_csc ${GS} ${S} ${H})
add_executable(graph500_mpi_custom bfs_custom.c ${SOURCES} ${HEADERS} ${GENERATOR_SOURCES})
target_link_libraries(graph500_mpi_custom ${GS} ${S} ${H})
