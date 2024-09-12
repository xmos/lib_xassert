cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(lib_xassert_examples)

add_subdirectory(app_assert)
add_subdirectory(app_unreachable)
add_subdirectory(app_fail)
