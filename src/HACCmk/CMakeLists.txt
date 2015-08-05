# :TODO:
# Automatically detect compiler and select flags
# Remove unnatural language from old Makefiles in favour of cleaner CMake style

# Create executable
add_executable(HACCmk main.c Step10_orig.c mysecond.c)

# Optimization flags
set (OPENMP_C_REPORT_FLAGS "") # gcc
# set(OPENMP_C_REPORT_FLAGS "-openmp-report2") # intel compiler linux
set(VERBOSE_OPTIMIZATION_FLAGS "-fopt-info-optall-optimized") # gcc
# set(VERBOSE_OPTIMIZATION_FLAGS "-vec-report6 -opt-report3") # intel v13 and v14 only. use -qopt-report interface for v15
set(FAST_MATH_FLAGS "-ffast-math") # gcc
# set(FAST_MATH_FLAGS "-no-prec-div") # intel
set(AVX_FLAGS "-march=core-avx2 -Wa,-q") # gcc using Clang assembler (MacOSX)
# set(AVX_FLAGS "-xAVX") # intel

# Define compiler flags
set(CFLAGS_MAIN     "-O0 ${OpenMP_C_FLAGS} ${OPENMP_C_REPORT_FLAGS} -Wall")
set(CFLAGS_KERNEL   "-O3 ${AVX_FLAGS} ${FAST_MATH_FLAGS} ${VERBOSE_OPTIMIZATION_FLAGS} -Wall")
set(CFLAGS_MYSECOND "-O0")
set(LDFLAGS         "-O0 ${OpenMP_C_FLAGS}")

# Link to OpenMP
target_link_libraries(HACCmk ${LDFLAGS})

# Timebase library
find_library(TIMEBASE_LIBRARY timebase)
if( NOT ("${TIMEBASE_LIBRARY}" STREQUAL "TIMEBASE_LIBRARY-NOTFOUND") )
	set(CFLAGS_MAIN "${CFLAGS_MAIN} -DTIMEBASE")
	target_link_libraries(HACCmk ${TIMEBASE_LIBRARY})
endif()

# Set compiler flags
set_source_files_properties(main.c        PROPERTIES COMPILE_FLAGS "${CFLAGS_MAIN}"  )
set_source_files_properties(Step10_orig.c PROPERTIES COMPILE_FLAGS "${CFLAGS_KERNEL}")
set_source_files_properties(mysecond.c    PROPERTIES COMPILE_FLAGS "${CFLAGS_MYSECOND}"  )

# Define variables for RunMe scripts
set(HACCMK_BG_SHAREDMEMSIZE 32)
set(HACCMK_L1P_POLICY "std")
set(HACCMK_NODES 1)
set(HACCMK_RANKS_PER_NODE 1)
set(HACCMK_BG_THREADLAYOUT 1)
math(EXPR HACCMK_NPROCS ${HACCMK_NODES}*${HACCMK_RANKS_PER_NODE})
set(HACCMK_OMP_LIST  01 02 04 08 16 32)
set(HACCMK_RUNME_IN "${CMAKE_CURRENT_SOURCE_DIR}/RunMe.sh.in")

# Loop over desired values of OMP_NUM_THREADS and configure individual RunMe scrips
foreach(HACCMK_OMP_NUM_THREADS ${HACCMK_OMP_LIST})
	set(HACCMK_JOB_${HACCMK_OMP_NUM_THREADS} "HACCmk_${HACCMK_NPROCS}_${HACCMK_OMP_NUM_THREADS}")
	set(HACCMK_VARS                          "FAST_WAKEUP=TRUE:BG_SHAREDMEMSIZE=${HACCMK_BG_SHAREDMEMSIZE}:OMP_NUM_THREADS=${HACCMK_OMP_NUM_THREADS}:L1P_POLICY=${HACCMK_L1P_POLICY}:BG_THREADLAYOUT=${HACCMK_BG_THREADLAYOUT}:XLSMPOPTS=stack=8000000")
	set(HACCMK_RUNME                         "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/${HACCMK_JOB_${HACCMK_OMP_NUM_THREADS}}.sh")
	configure_file(${HACCMK_RUNME_IN} ${HACCMK_RUNME})
	file(GENERATE OUTPUT ${HACCMK_RUNME} INPUT ${HACCMK_RUNME})
endforeach()

# Configure RunAll script
set(HACCMK_RUNALL_IN "${CMAKE_CURRENT_SOURCE_DIR}/RunAll.in")
set(HACCMK_RUNALL    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/RunAll")
configure_file(${HACCMK_RUNALL_IN} ${HACCMK_RUNALL})

# Configure CheckAll script
set(HACCMK_CHECKALL_IN "${CMAKE_CURRENT_SOURCE_DIR}/CheckAll.in")
set(HACCMK_CHECKALL    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/CheckAll")
configure_file(${HACCMK_CHECKALL_IN} ${HACCMK_CHECKALL})
