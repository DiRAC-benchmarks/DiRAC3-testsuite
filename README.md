# DiRAC3-testsuite

## Dependencies

* [CMake 3.x](https://cmake.org/download/#latest)
* [METIS 5.x](http://glaros.dtc.umn.edu/gkhome/metis/metis/download)
  * n.b. SWIFT will build without METIS, but with impaired load balance
* [libtool](https://www.gnu.org/software/libtool/)
* [Automake >= 1.11](http://www.gnu.org/software/autoconf/autoconf.html)

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
module swap intel intel/15.0.2.164
CRAYPE_LINK_TYPE=dynamic FC=ftn cmake .. -DDIRAC3_PRIVATE=TRUE
CRAYPE_LINK_TYPE=dynamic make all
```

Then proceed as above.
