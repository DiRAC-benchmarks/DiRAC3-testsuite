# DiRAC3-testsuite

## Benchmarks

### Extreme-Scaling

* [Grid](https://github.com/paboyle/Grid/) - Lattice quantum chromodynamics
* [MILCmk](https://asc.llnl.gov/CORAL-benchmarks/) - Lattice quantum chromodynamics
* [QPhiX](https://jeffersonlab.github.io/qphix/) - Lattice quantum chromodynamics

### Memory-Intensive

* [CloverLeaf3D](http://uk-mac.github.io/CloverLeaf3D/) - Magnetohydrodynamics on a structured grid
* [Gadget3-Eagle](http://icc.dur.ac.uk/Eagle/) (Private) - Smoothed-particle hydrodynamics
* [Stream](http://www.cs.virginia.edu/stream/) - Streaming memory bandwidth
* [Stride](https://asc.llnl.gov/CORAL-benchmarks/) - Strided memory bandwidth
* [Swift](http://icc.dur.ac.uk/swift/) - Smoothed-particle hydrodynamics

### Data-Intensive

* [IOR](https://sourceforge.net/projects/ior-sio/) - Filesystem I/O
* [Walls](http://www.damtp.cam.ac.uk/research/gr/public/cs_evol.html) (Private) - Cosmological domain walls on a structured grid

## Dependencies

* [CMake 3.x](https://cmake.org/download/)
* [Python 2.7.x](https://www.python.org/downloads/)
* [libtool](https://www.gnu.org/software/libtool/)
* [Automake >= 1.11](http://www.gnu.org/software/automake/)
* [HDF5](https://www.hdfgroup.org/downloads/index.html) (including h5cc and h5pcc for Swift)
* [METIS 5.x](http://glaros.dtc.umn.edu/gkhome/metis/metis/download) (recommended, improves Swift load balance)

## Other requirements

SWIFT expects ```$CC``` and ```mpicc``` to have the same 'flavour' e.g. Intel, GCC, ...
  * You *might* need to set the ```I_MPI_CC``` environment variable for Intel MPI (as on DiRAC Complexity)
  * If (as on DiRAC COSMOS) the system ```mpicc``` cannot be configured, place a shell script like this before it in the path:

    ```
    #!/bin/sh
    icc "$@" # append MPI compiler, linker options as required
    ```

  * n.b. ```configure``` sets the environment variable ```CC=mpicc```, so the compiler (here ```icc```) must be named directly.

## Module File and Submission Script Template

Before building the benchmarks on a system, two files must be created for a `hostname` of your choice:

* Create, modify and source a script named `modules/modules.hostname` that loads necessary modules and sets other environment variables.
* Create and modify a template script named `templates/submit.hostname` for submitting benchmarks to a batch job scheduler.

Sample module and submission template scripts are provided.

## Build Instructions

The testsuite is downloaded, configured and built as below, passing the chosen `hostname` to cmake:

```
git clone --recursive git@github.com:DiRAC-benchmarks/DiRAC3-testsuite.git
cd DiRAC3-testsuite
mkdir build
cd build
cmake .. -DDIRAC3_HOST=hostname -DDIRAC3_PRIVATE=TRUE
make all
```

## Additional configuration for Archer Cray XC30

The benchmarks have been tested on the [Archer UK National Supercomputing Service](http://www.archer.ac.uk/) using the modules listed in `modules/modules.archer`, including Intel compilers. The following additional configurations were necessary:

* The ```/home``` filesystem is not available on the compute nodes; we recommended building from source in ```/work```.

* To use the Fortran compiler wrapper, set ```FC=ftn``` when calling cmake.

* With Intel compilers, set ```CRAYPE_LINK_TYPE=dynamic``` when calling cmake and make.

* Archer's HDF5 module does not have the ```h5cc``` and ```h5pcc``` scripts required by Swift. A workaround is to create both scripts in ```PATH``` with the following body:

  ```
  #!/bin/sh
  cc "$@"
  ```

* To use ```aprun```, make the following changes to `CMakeLists.txt` before and after the call to ```find_package(MPI REQUIRED)```:

  ```
  find_program(MPIEXEC aprun)
  find_package(MPI REQUIRED)
  set(MPIEXEC_NUMPROC_FLAG -n)
  set(MPIEXEC_PREFLAGS "-d $OMP_NUM_THREADS -cc numa_node")
  ```

  (Note that the flag ```-cc numa_node``` is only needed with Intel compilers).

* With Intel compilers, add the line ```export KMP_AFFINITY=disabled``` to `templates/submit.archer.in`.

* There is a conflict between the fftw2 library modules and the Intel programming environment when using CMake. We recommended passing the fftw2 installation path to CMake using the variable `FFTW2_ROOT`.

The final build command is then:

```
cd build
source ../modules/modules.archer
CRAYPE_LINK_TYPE=dynamic FC=ftn cmake .. -DDIRAC3_HOST=archer -DDIRAC3_PRIVATE=TRUE -DFFTW2_ROOT=/opt/cray/fftw/2.1.5.9
CRAYPE_LINK_TYPE=dynamic make all
```
