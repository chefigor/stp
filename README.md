# STP

## Prerequisites

* [SWIG](https://github.com/swig/swig)
* [CAF(C++ actor framework)](https://github.com/actor-framework/actor-framework)
* [spdlog](https://github.com/gabime/spdlog)
* CMake
  
## Building instructions 

```shell
mkdir build && cd build
cmake ..
make -j
ctest
```
#### Testing
```shell
ctest
ctest -T memcheck
```
