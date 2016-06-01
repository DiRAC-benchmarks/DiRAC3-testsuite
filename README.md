# DiRAC3-testsuite

## Dependencies

* [CMake 3.x](https://cmake.org/download/)
* [Python 2.7.x](https://www.python.org/downloads/)
* [METIS 5.x](http://glaros.dtc.umn.edu/gkhome/metis/metis/download)
  * n.b. SWIFT will build without METIS, but with impaired load balance
* [libtool](https://www.gnu.org/software/libtool/)
* [Automake >= 1.11](http://www.gnu.org/software/automake/)
* [HDF5](https://www.hdfgroup.org/downloads/index.html) (inclding h5cc and h5pcc for Swift)

## Other requirements

* SWIFT expects ```$CC``` and ```mpicc``` to have the same 'flavour' e.g. Intel, GCC, ...
  * You *might* need to set the ```I_MPI_CC``` environment variable for Intel MPI (as on DiRAC Complexity)
  * If (as on DiRAC COSMOS) the system ```mpicc``` cannot be configured, place a shell script like this before it in the path:
   ```
   #!/bin/sh
   icc "$@" # append MPI compiler, linker options as required
   ```
  * n.b. ```configure``` sets the environment variable ```CC=mpicc```, so the compiler (here ```icc```) must be named directly.

## To build benchmarks...

```
git clone --recursive git@github.com:DiRAC-benchmarks/DiRAC3-testsuite.git
cd DiRAC3-testsuite
mkdir bin build run scripts
cd build
cmake .. -DDIRAC3_PRIVATE=TRUE
make all
```

### Additional build instructions for Archer Cray XC30

```
module swap PrgEnv-cray PrgEnv-intel
module swap intel intel/16.0.2.181
module load gcc
module load python-compute
module load autotools
module load metis
CRAYPE_LINK_TYPE=dynamic FC=ftn cmake .. -DDIRAC3_PRIVATE=TRUE
CRAYPE_LINK_TYPE=dynamic LDFLAGS=-L/path/to/libhdf5 make all
```

Note that the HDF5 module on Archer does not include the h5cc and h5pcc scripts required by the Swift benchmark. A local version of the library should be built, ensuring that h5cc and h5pcc are in PATH and the directory containing libhdf5 is added to the link path using LDFLAGS when calling make.
