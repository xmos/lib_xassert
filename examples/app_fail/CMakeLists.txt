cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(app_fail)

option(XCOREAI "Build for xcore.ai instead of xcore-200" ON)

if(XCOREAI)
    set(APP_HW_TARGET XCORE-AI-EXPLORER)
else()
    set(APP_HW_TARGET XCORE-200-EXPLORER)
endif()

set(APP_DEPENDENT_MODULES "lib_xassert")

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../)

XMOS_REGISTER_APP()
