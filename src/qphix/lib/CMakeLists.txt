cmake_minimum_required (VERSION 2.6)
project(qphix/lib C CXX)

SET(TOPDIR "${CMAKE_SOURCE_DIR}")
#TOPDIR=@top_srcdir@
#BUILDDIR=@top_builddir@
include_directories(${TOPDIR}/include)
SET(BUILDDIR "${TOPDIR}/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${TOPDIR}/include -I${BUILDDIR}/include ${QPHIX_AUX_CFLAGS}")
#AM_CXXFLAGS= -I$(TOPDIR)/include -I$(BUILDDIR)/include @QPHIX_AUX_CFLAGS@
#lib_LIBRARIES = libqphix_solver.a
#libqphix_solver_a_SOURCES = \
#	print_utils.cc
set(libqphix_solver_a_SOURCES ${CMAKE_SOURCE_DIR}/lib/print_utils.cc)
if(${QPHIX_BUILD_QPX})
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} bgq_threadbind.cc)
else()
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} ${CMAKE_SOURCE_DIR}/lib/generic_threadbind.cc)
endif()

add_library(qphix_solver STATIC ${libqphix_solver_a_SOURCES})
