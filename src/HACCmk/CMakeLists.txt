find_package(OpenMP REQUIRED)
add_executable(HACCmk main.c Step10_orig.c mysecond.c)

# These should be determined automatically

set (OPENMP_C_REPORT_FLAGS "") # gcc
# set(OPENMP_C_REPORT_FLAGS "-openmp-report2") # intel compiler linux

set(VERBOSE_OPTIMIZATION_FLAGS "-fopt-info-optall-optimized") # gcc
# set(VERBOSE_OPTIMIZATION_FLAGS "-vec-report6 -opt-report3") # intel v13 and v14 only. use -qopt-report interface for v15

set(FAST_MATH_FLAGS "-ffast-math") # gcc
# set(FAST_MATH_FLAGS "-no-prec-div") # intel

set(AVX_FLAGS "-march=core-avx2 -Wa,-q") # gcc using Clang assembler (MacOSX)
# set(AVX_FLAGS "-xAVX") # intel

set(CFLAGS_MAIN     "-O0 ${OpenMP_C_FLAGS} ${OPENMP_C_REPORT_FLAGS} -Wall")
set(CFLAGS_KERNEL   "-O3 ${AVX_FLAGS} ${FAST_MATH_FLAGS} ${VERBOSE_OPTIMIZATION_FLAGS} -Wall")
set(CFLAGS_MYSECOND "-O0")
set(LDFLAGS         "-O0 ${OpenMP_C_FLAGS}")

target_link_libraries(HACCmk ${LDFLAGS})

find_library(TIMEBASE_LIBRARY timebase)
IF( NOT ("${TIMEBASE_LIBRARY}" STREQUAL "TIMEBASE_LIBRARY-NOTFOUND") )
	set(CFLAGS_MAIN "${CFLAGS_MAIN} -DTIMEBASE")
	target_link_libraries(HACCmk ${TIMEBASE_LIBRARY})
ENDIF()

set_source_files_properties(main.c        PROPERTIES COMPILE_FLAGS "${CFLAGS_MAIN}"  )
set_source_files_properties(Step10_orig.c PROPERTIES COMPILE_FLAGS "${CFLAGS_KERNEL}")
set_source_files_properties(mysecond.c    PROPERTIES COMPILE_FLAGS "${CFLAGS_MAIN}"  )
