cmake_minimum_required (VERSION 2.6)
project(qphix/lib)

SET(TOPDIR "../")
#TOPDIR=@top_srcdir@
#BUILDDIR=@top_builddir@
SET(BUILDDIR "${TOPDIR}/build")

include_directories(${TOPDIR}/include)
include_directories(${BUILDDIR}/include)


#INCFLAGS=-I@top_srcdir@/include -I@top_builddir@/include

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${QPHIX_AUX_CFLAGS}")

#AM_CXXFLAGS=$(INCFLAGS) @QPHIX_AUX_CFLAGS@

set(CMAKE_EXE_LINKER_FLAGS "-L${TOPDIR}/lib ${QPHIX_AUX_LDFLAGS} ${QPHIX_AUX_LIBS}")

#AM_LDFLAGS=-L@top_builddir@/lib @LDFLAGS@ @QPHIX_AUX_LDFLAGS@
#LDADD = -lqphix_solver @LIBS@ @QPHIX_AUX_LIBS@

#check_PROGRAMS = time_dslash_noqdp
#if QPHIX_BUILD_WITH_QDP
#check_PROGRAMS += t_dslash t_minvcg
#endif

#if QPHIX_BUILD_CLOVER
#check_PROGRAMS += time_clov_noqdp
#if QPHIX_BUILD_WITH_QDP
#check_PROGRAMS +=  t_clov_dslash t_clov_invert_from_file
#endif
#endif

# check_PROGRAMS += test_blas




# bin_PROGRAMS = $(check_PROGRAMS)
#
# FRAMEWORK_SRCS =  unittest.h \
# 	dslashm_w.h \
# 	dslashm_w.cc \
# 	reunit.h \
# 	invbicgstab_test.h \
# 	reunit.cc
#
# t_dslash_SOURCES = $(FRAMEWORK_SRCS) \
# 	testDslashFull.h \
# 	testDslashFull.cc \
# 	t_dslash.cc


add_executable(time_dslash_noqdp 	time_dslash_noqdp.cc 	timeDslashNoQDP.h 	timeDslashNoQDP.cc)
target_link_libraries(time_dslash_noqdp qphix_solver)
# time_dslash_noqdp_SOURCES= \
# 	time_dslash_noqdp.cc \
# 	timeDslashNoQDP.h \
# 	timeDslashNoQDP.cc
#
# t_minvcg_SOURCES = $(FRAMEWORK_SRCS) \
# 	testMInvCG.h \
# 	testMInvCG.cc \
# 	t_minvcg.cc
#
# t_dslash_DEPENDENCIES = build_lib
# time_dslash_noqdp_DEPENDENCIES=build_lib
# t_minvcg_DEPENDENCIES=build_lib
#
#
#
#
# if QPHIX_BUILD_CLOVER
# t_clov_dslash_SOURCES = $(FRAMEWORK_SRCS) \
# 	testClovDslashFull.h \
# 	testClovDslashFull.cc \
# 	t_clov_dslash.cc \
# 	mesfield.h \
# 	mesfield.cc \
# 	clover_term_qdp_w.h \
# 	clover_fermact_params_w.h
#
# t_clov_invert_from_file_SOURCES = $(FRAMEWORK_SRCS) \
# 	testClovInvertFromFile.h \
# 	testClovInvertFromFile.cc \
# 	t_clov_invert_from_file.cc \
# 	mesfield.h \
# 	mesfield.cc \
# 	clover_term_qdp_w.h \
# 	clover_fermact_params_w.h
#
# time_clov_noqdp_SOURCES= \
# 	time_clov_noqdp.cc \
# 	timeClovNoQDP.h \
# 	timeClovNoQDP.cc
#
# t_clov_dslash_DEPENDENCIES = build_lib
# t_clov_invert_from_file_DEPENDENCIES = build_lib
# time_clov_noqdp_DEPENDENCIES = build_lib
#
# endif
#
# #test_blas_SOURCES=$(FRAMEWORK_SRCS) \
# #	test_blas.cc \
# #	testBlas.h \
# #	testBlas.cc
# #
# #test_blas_DEPENDENCIES= build_lib
#
# DEPENDENCIES = build_lib
# ${bin_PROGRAMS}: ${DEPENDENCIES}
# ${check_PROGRAMS}: ${DEPENDENCIES}
# ${EXTRA_PROGRAMS}: ${DEPENDENCIES}
#
# .PHONY: build_lib
# 	cd @top_builddir@/lib; $(MAKE)
