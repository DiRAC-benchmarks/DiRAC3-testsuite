cmake_minimum_required (VERSION 2.6)
project(qphix/lib C CXX)

SET(TOPDIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
#TOPDIR=@top_srcdir@

SET(BUILDDIR "${TOPDIR}/build")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")
set(libqphix_solver_a_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/print_utils.cc)
if(${QPHIX_BUILD_QPX})
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/bgq_threadbind.cc)
else()
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/generic_threadbind.cc)
endif()

add_library(qphix_solver STATIC ${libqphix_solver_a_SOURCES})
