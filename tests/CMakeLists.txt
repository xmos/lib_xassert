cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(lib_xassert_tests)

add_subdirectory(assert_test)
add_subdirectory(assert_test_unit)
add_subdirectory(fail_test)
add_subdirectory(unreachable_test)
