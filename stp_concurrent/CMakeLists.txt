add_library(stp_concurrent src/stp_concurrent.cpp)
target_include_directories(stp_concurrent PRIVATE include)
target_link_libraries(stp_concurrent PRIVATE pthread)
target_link_libraries(stp_concurrent PRIVATE spdlog::spdlog_header_only)
target_compile_options(stp_concurrent PRIVATE -Werror -Wall -Wextra)
target_compile_features(stp_concurrent PRIVATE cxx_std_17)

add_executable(test_concurrent test/test.cpp)
target_include_directories(test_concurrent PRIVATE include)
target_link_libraries(test_concurrent PRIVATE stp_concurrent)
target_link_libraries(test_concurrent PRIVATE gtest_main)
if($CACHE{VALGRIND_TEST})
  add_custom_command(TARGET test_concurrent POST_BUILD COMMAND "${VALGRIND}"  --tool=helgrind --log-file="helgrind.out"  $<TARGET_FILE:test_concurrent>) 
endif()

include(GoogleTest)
gtest_discover_tests(test_concurrent)
