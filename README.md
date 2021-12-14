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
ctest -j10
ctest -R memcheck -j10
ctest -R helgrind -j10
```
