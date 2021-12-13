# STP

## Prerequisites

* [SWIG](https://github.com/swig/swig)
* [CAF(C++ actor framework)](https://github.com/actor-framework/actor-framework)
* [spdlog](https://github.com/gabime/spdlog)
* [gtest](https://github.com/google/googletest)
* CMake
  
## Building instructions 

```shell
mkdir build && cd build
cmake ..
make -j
```
#### Testing
```shell
ctest
ctest -T memcheck
```
Custom cmake target for valgrind tests enabled with option VALGRIN_TEST=true and results displayed in file helgrind.out 
