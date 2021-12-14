add_library(stp_caf src/stp_caf.cpp)
find_package(CAF REQUIRED)
target_include_directories(stp_caf PRIVATE include)
target_link_libraries(stp_caf PRIVATE CAF::core)
target_compile_options(stp_caf PRIVATE -Werror -Wall -Wextra)
target_link_libraries(stp_caf PRIVATE spdlog::spdlog_header_only)

add_executable(test_caf test/test.cpp)
target_include_directories(test_caf PRIVATE include)
target_link_libraries(test_caf PRIVATE CAF::core CAF::test)
add_test(test_caf.convergence test_caf)

add_test(
  NAME memcheck_caf
  COMMAND
    ${VALGRIND} --tool=memcheck --xml=yes
    --xml-file=${VALGRIND_PATH}/memcheck_caf.xml $<TARGET_FILE:test_caf>)
add_test(
  NAME helgrind_caf
  COMMAND
    ${VALGRIND} --tool=helgrind --xml=yes
    --xml-file=${VALGRIND_PATH}/helgrind_caf.xml $<TARGET_FILE:test_caf>)
