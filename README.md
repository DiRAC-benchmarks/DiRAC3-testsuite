# DiRAC3-testsuite

Requirements:

* CMake v3.0

Build benchmarks:

```
git clone --recursive git@github.com:DiRAC-benchmarks/DiRAC3-testsuite.git
cd DiRAC3-testsuite
mkdir bin build run scripts
cd build
cmake .. -DDIRAC3_PRIVATE=TRUE
make all
```

Archer Cray XC30 build:

```
module swap PrgEnv-cray PrgEnv-intel
module swap intel intel/15.0.2.164
CRAYPE_LINK_TYPE=dynamic FC=ftn cmake .. -DDIRAC3_PRIVATE=TRUE
CRAYPE_LINK_TYPE=dynamic make all
```
