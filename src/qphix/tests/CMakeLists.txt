cmake_minimum_required (VERSION 2.6)
project(qphix/lib)

SET(TOPDIR "../")
SET(BUILDDIR "${TOPDIR}/build")

include_directories(${TOPDIR}/include)
include_directories(${BUILDDIR}/include)
include_directories(${QMP_HOME}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "-L${TOPDIR}/lib ${QPHIX_AUX_LDFLAGS} ${QPHIX_AUX_LIBS}")

add_executable(time_dslash_noqdp 	time_dslash_noqdp.cc 	timeDslashNoQDP.h 	timeDslashNoQDP.cc)
target_link_libraries(time_dslash_noqdp qphix_solver ${QMP_LIBRARY})
