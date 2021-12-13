cmake_minimum_required(VERSION 3.2.0)
project(
  STP
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

include(CTest)

set(VALGRIND_TEST CACHE STRING FALSE)
find_program(VALGRIND "valgrind")

find_package(spdlog REQUIRED)

add_subdirectory(stp_caf)
add_subdirectory(stp_concurrent)
add_subdirectory(stp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/3e0e32ba300ce8afe695ad3ba7e81b21b7cf237a.zip
)

FetchContent_MakeAvailable(googletest)

include(cmake/CPM.cmake)

cpmaddpackage(
  NAME
  Format.cmake
  VERSION
  1.7.0
  GITHUB_REPOSITORY
  TheLartians/Format.cmake
  OPTIONS # set to yes skip cmake formatting
  "FORMAT_SKIP_CMAKE NO"
  # path to exclude (optional, supports regular expressions)
  "CMAKE_FORMAT_EXCLUDE cmake/CPM.cmake")
