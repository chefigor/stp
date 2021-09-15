cmake_minimum_required(VERSION 3.0.0)
project(STP VERSION 0.1.0)

add_executable(stp stp.cpp)
find_package(CAF REQUIRED)
target_link_libraries(stp PRIVATE CAF::internal CAF::core)
# target_link_libraries(stp
# libcaf_io
# libcaf_core
# )