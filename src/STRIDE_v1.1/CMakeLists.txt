# :TODO:
# Script files should be targets with custom commands so that make clean removes them
# Construction of executables inside for loops should be a single function

# Build targets for optimized and unoptimized executables
add_custom_target(stride-optimized)
add_custom_target(stride-unoptimized)

# Single build target for all executables
add_custom_target(stride-benchmarks)
add_dependencies(stride-benchmarks stride-optimized stride-unoptimized)

# Optimized Compiler Flags
set(STRIDE_FFLAGS  "-O3 -DLINUX")
set(STRIDE_CFLAGS  "-O3 -DLINUX")

# Unoptimized Compiler Flags
set(STRIDE_GFFLAGS "-g  -DLINUX")
set(STRIDE_GCFLAGS "-g  -DLINUX")

# Libraries
set(STRIDE_LDCFLAGS "")
set(STRIDE_LDFFLAGS "")

# Object Files
add_library           (STRIDE_SECC OBJECT secc.c)
set_target_properties (STRIDE_SECC PROPERTIES COMPILE_FLAGS "${STRIDE_CFLAGS}")
add_library           (STRIDE_SECF OBJECT secf.c)
set_target_properties (STRIDE_SECF PROPERTIES COMPILE_FLAGS "${STRIDE_CFLAGS}")

# Fortran Source
file(GLOB STRIDE_FSOURCE *.f)

# Fortran Executables from each file
foreach(STRIDE_FFILE ${STRIDE_FSOURCE})

	# Optimized Executable
	get_filename_component (STRIDE_FTARGET "${STRIDE_FFILE}" NAME_WE)
	set                    (STRIDE_FTARGET "${STRIDE_FTARGET}.Opt")
	add_executable         (${STRIDE_FTARGET} ${STRIDE_FFILE} $<TARGET_OBJECTS:STRIDE_SECF>)
	set_target_properties  (${STRIDE_FTARGET} PROPERTIES COMPILE_FLAGS "${STRIDE_FFLAGS}")
	target_link_libraries  (${STRIDE_FTARGET} ${STRIDE_LDFFLAGS})
	add_dependencies       (stride-optimized ${STRIDE_FTARGET})

	# Unoptimized Executable
	get_filename_component (STRIDE_FGTARGET "${STRIDE_FFILE}" NAME_WE)
	set                    (STRIDE_FGTARGET "${STRIDE_FGTARGET}.g")
	add_executable         (${STRIDE_FGTARGET} ${STRIDE_FFILE} $<TARGET_OBJECTS:STRIDE_SECF>)
	set_target_properties  (${STRIDE_FGTARGET} PROPERTIES COMPILE_FLAGS "${STRIDE_GFFLAGS}")
	target_link_libraries  (${STRIDE_FGTARGET} ${STRIDE_LDFFLAGS})
	add_dependencies       (stride-unoptimized ${STRIDE_FGTARGET})

endforeach()

# C Source
file(GLOB STRIDE_CSOURCE *.c)
file(GLOB STRIDE_CSOURCE_REMOVE secc.c secf.c)
list(REMOVE_ITEM STRIDE_CSOURCE ${STRIDE_CSOURCE_REMOVE})

# C Executables from each file
foreach(STRIDE_CFILE ${STRIDE_CSOURCE})

	# Optimized Executable
	get_filename_component (STRIDE_CTARGET "${STRIDE_CFILE}" NAME_WE)
	set                    (STRIDE_CTARGET "${STRIDE_CTARGET}c.Opt")
	add_executable         (${STRIDE_CTARGET} ${STRIDE_CFILE} $<TARGET_OBJECTS:STRIDE_SECC>)
	set_target_properties  (${STRIDE_CTARGET} PROPERTIES COMPILE_FLAGS "${STRIDE_CFLAGS}")
	target_link_libraries  (${STRIDE_CTARGET} ${STRIDE_LDCFLAGS})
	add_dependencies       (stride-optimized ${STRIDE_CTARGET})

	# Unoptimized Executable
	get_filename_component (STRIDE_CGTARGET "${STRIDE_CFILE}" NAME_WE)
	set                    (STRIDE_CGTARGET "${STRIDE_CGTARGET}c.g")
	add_executable         (${STRIDE_CGTARGET} ${STRIDE_CFILE} $<TARGET_OBJECTS:STRIDE_SECC>)
	set_target_properties  (${STRIDE_CGTARGET} PROPERTIES COMPILE_FLAGS "${STRIDE_GCFLAGS}")
	target_link_libraries  (${STRIDE_CGTARGET} ${STRIDE_LDCFLAGS})
	add_dependencies       (stride-unoptimized ${STRIDE_CGTARGET})

endforeach()

# Generate run.cache script
set(STRIDE_RUN_CACHE_IN "${CMAKE_CURRENT_SOURCE_DIR}/run.cache.in")
set(STRIDE_RUN_CACHE    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/run.cache")
file(GENERATE OUTPUT ${STRIDE_RUN_CACHE} INPUT ${STRIDE_RUN_CACHE_IN})

# Generate script.cache script
set(STRIDE_SCRIPT_CACHE_IN "${CMAKE_CURRENT_SOURCE_DIR}/script.cache.in")
set(STRIDE_SCRIPT_CACHE    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/script.cache")
file(GENERATE OUTPUT ${STRIDE_SCRIPT_CACHE} INPUT ${STRIDE_SCRIPT_CACHE_IN})

# Configure runit script
set(STRIDE_RUNIT_IN "${CMAKE_CURRENT_SOURCE_DIR}/runit.in")
set(STRIDE_RUNIT    "${DIRAC3_TESTSUITE_SCRIPTS_PATH}/runit")
configure_file(${STRIDE_RUNIT_IN} ${STRIDE_RUNIT})
