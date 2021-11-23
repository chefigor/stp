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
For now there are no tests for CAF or MoodyCamel implementation, to see those in action: 
```shell
cd build/stp_caf && ./stp_caf
cd build/stp_concurrent && ./stp_concurrent
```
