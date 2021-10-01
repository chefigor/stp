cmake_minimum_required(VERSION 3.0.0)
project(STP VERSION 0.1.0)

add_executable(stp stp.cpp)
find_package(CAF REQUIRED)
target_link_libraries(stp PRIVATE CAF::internal CAF::core)
# target_link_libraries(stp
# libcaf_io
# libcaf_core
# )
add_executable(stp_c stp_c.cpp)
target_link_libraries(stp_c PRIVATE pthread)
set_property(TARGET stp_c PROPERTY CXX_STANDARD 17)

add_executable(stp_concurrent stp_concurrent.cpp)
target_link_libraries(stp_concurrent PRIVATE pthread)
