cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(app_assert)

set(APP_HW_TARGET XK-EVK-XU316)

include(${CMAKE_CURRENT_LIST_DIR}/../deps.cmake)

set(APP_COMPILER_FLAGS -DXASSERT_ENABLE_ASSERTIONS=0)   # Override from source files

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../)

XMOS_REGISTER_APP()
