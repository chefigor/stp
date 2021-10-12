include(FindSWIG REQUIRED)
include(UseSWIG REQUIRED)
find_package(PythonLibs REQUIRED)

#set(SWIG_SOURCE_FILE_EXTENSIONS ".i" ".ipp")
set_property(SOURCE src/stp.i PROPERTY CPLUSPLUS ON)
swig_add_library(stp
LANGUAGE python
SOURCES src/stp.i)
target_include_directories(stp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src ${PYTHON_INCLUDE_DIRS})

add_library(lib_stp SHARED src/stp.cpp)
target_compile_options(lib_stp PRIVATE -Werror -Wall -Wextra)
target_compile_features(lib_stp PRIVATE cxx_std_17)
swig_link_libraries(stp lib_stp)