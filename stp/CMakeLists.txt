include(FindSWIG REQUIRED)
include(UseSWIG REQUIRED)
find_package(PythonLibs REQUIRED)

# set(SWIG_SOURCE_FILE_EXTENSIONS ".i" ".ipp")
set_property(SOURCE src/stp.i PROPERTY CPLUSPLUS ON)
swig_add_library(
  stp
  LANGUAGE python
  SOURCES src/stp.i)
target_include_directories(stp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
                                       ${PYTHON_INCLUDE_DIRS})

add_library(_stp SHARED src/stp.cpp)
target_link_libraries(_stp PRIVATE spdlog::spdlog_header_only)
target_compile_options(_stp PRIVATE -Werror -Wall -Wextra)
swig_link_libraries(stp _stp)

find_package(Python REQUIRED)
add_test(
  NAME test_stp_python
  COMMAND ${Python_EXECUTABLE} -m unittest
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test)
set_tests_properties(
  test_stp_python
  PROPERTIES ENVIRONMENT
             "PYTHONPATH=${CMAKE_CURRENT_BINARY_DIR}:$ENV{PYTHONPATH}")
