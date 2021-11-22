cmake_minimum_required(VERSION 3.2.0)
project(
  STP
  VERSION 1.0.0
  LANGUAGES CXX)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

include(CTest)

find_package(spdlog REQUIRED)
add_subdirectory(stp_caf)
add_subdirectory(stp_concurrent)
add_subdirectory(stp)

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
