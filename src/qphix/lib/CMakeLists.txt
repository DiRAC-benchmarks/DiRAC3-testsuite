cmake_minimum_required (VERSION 2.6)
project(qphix/lib C CXX)

SET(TOPDIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
#TOPDIR=@top_srcdir@
<<<<<<< HEAD

SET(BUILDDIR "${TOPDIR}/build")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")
=======
#BUILDDIR=@top_builddir@
include_directories(${TOPDIR}/include)
include_directories(${BUILDDIR}/include)
SET(BUILDDIR "${TOPDIR}/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")
#AM_CXXFLAGS= -I$(TOPDIR)/include -I$(BUILDDIR)/include @QPHIX_AUX_CFLAGS@
#lib_LIBRARIES = libqphix_solver.a
#libqphix_solver_a_SOURCES = \
#	print_utils.cc
>>>>>>> 970c828c93215e8041a32cf07937be83ea8df086
set(libqphix_solver_a_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/print_utils.cc)
if(${QPHIX_BUILD_QPX})
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/bgq_threadbind.cc)
else()
	SET(libqphix_solver_a_SOURCES ${libqphix_solver_a_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/generic_threadbind.cc)
endif()

add_library(qphix_solver STATIC ${libqphix_solver_a_SOURCES})
