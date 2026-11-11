cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(lib_xassert_examples)

option(XASSERT_BUILD_XC_EXAMPLES "Build XS-only XC examples" ON)

add_subdirectory(app_assert)
add_subdirectory(app_unreachable)
add_subdirectory(app_fail)
add_subdirectory(app_timed_block)
add_subdirectory(app_timed_loop)

if(XASSERT_BUILD_XC_EXAMPLES)
  add_subdirectory(app_timing_loop_exception)
endif()
