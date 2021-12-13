include(FindSWIG REQUIRED)
include(UseSWIG REQUIRED)
find_package(PythonLibs REQUIRED)

add_library(_stp SHARED src/stp.cpp include/stp.hpp)
target_include_directories(_stp PUBLIC include)
target_link_libraries(_stp PRIVATE spdlog::spdlog_header_only)
target_compile_options(_stp PRIVATE -Werror -Wall -Wextra)
set_target_properties(_stp PROPERTIES PUBLIC_HEADER include/stp.hpp)

set_property(SOURCE src/stp.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE src/stp.i PROPERTY SWIG_MODULE_NAME stp)

swig_add_library(
  stp
  TYPE SHARED
  LANGUAGE python
  SOURCES src/stp.i)
target_include_directories(stp PRIVATE include/ ${CMAKE_CURRENT_SOURCE_DIR}/src
                                       ${PYTHON_INCLUDE_DIRS})
set_property(TARGET stp PROPERTY SWIG_USE_TARGET_INCLUDE_DIRECTORIES ON)
target_link_libraries(stp PRIVATE _stp)

find_package(Python REQUIRED)
add_test(
  NAME test_stp_py.convergence
  COMMAND ${Python_EXECUTABLE} -m unittest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
set_tests_properties(
  test_stp_py.convergence
  PROPERTIES ENVIRONMENT
             "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PYTHONPATH}")

add_executable(test_stp_g test/test.cpp)
target_link_libraries(test_stp_g PRIVATE _stp)
target_link_libraries(test_stp_g PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(test_stp_g)

add_test(
  NAME memcheck_stp_g
  COMMAND
    ${VALGRIND} --tool=memcheck --xml=yes
    --xml-file=${VALGRIND_PATH}/memcheck_stp_g.xml $<TARGET_FILE:test_stp_g>)
add_test(
  NAME helgrind_stp_g
  COMMAND
    ${VALGRIND} --tool=helgrind --xml=yes
    --xml-file=${VALGRIND_PATH}/helgrind_stp_g.xml $<TARGET_FILE:test_stp_g>)
