cmake_minimum_required(VERSION 3.2.0)
project(STP VERSION 1.0.0 LANGUAGES CXX)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

add_subdirectory(stp_caf)
add_subdirectory(stp_concurrent)
add_subdirectory(stp)