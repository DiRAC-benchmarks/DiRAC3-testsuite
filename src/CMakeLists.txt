#add_subdirectory(ior)
add_subdirectory(MILCmk-v1)
# add_subdirectory(qphix)
add_subdirectory(stream_omp_v1.0)
add_subdirectory(STRIDE_v1.1)

if(DIRAC_BUILD_GADGET)
  add_subdirectory(Gadget3EagleBench)
endif(DIRAC_BUILD_GADGET)

if(DIRAC_BUILD_GRID)
  add_subdirectory(Grid)
endif(DIRAC_BUILD_GRID)
