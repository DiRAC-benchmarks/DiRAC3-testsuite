# DiRAC3-testsuite

Requirements:

* CMake v3.0

Build benchmarks:

```
git clone --recursive git@github.com:DiRAC-benchmarks/DiRAC3-testsuite.git
cd DiRAC3-testsuite
mkdir bin build run scripts
cmake .. -DDIRAC3_PRIVATE=TRUE
make all
```

Archer Cray XC30 build:

```
CRAYPE_LINK_TYPE=dynamic FC=ftn cmake .. -DDIRAC3_PRIVATE=TRUE
CRAYPE_LINK_TYPE=dynamic make all
```
