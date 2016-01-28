# DiRAC3-testsuite

Requirements:

* CMake v3.0

Build benchmarks:

```
git clone --recursive https://github.com/DiRAC-benchmarks/DiRAC3-testsuite.git
cd DiRAC3-testsuite
mkdir bin
mkdir build
cd build
cmake .. -DDIRAC3_PRIVATE=TRUE
make all
```
