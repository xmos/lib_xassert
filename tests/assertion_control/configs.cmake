# Build the same control matrix for C and, on XS, XC.
set(APP_INCLUDES ${CMAKE_CURRENT_LIST_DIR})

macro(add_control_profile name global_assertions global_timing assertions timing)
  foreach(probe REGULAR TIMING LOOP STATE)
    set(APP_COMPILER_FLAGS_${name}_${probe}
        -O3 -Wall
        -DXASSERT_ENABLE_ASSERTIONS=${global_assertions}
        -DXASSERT_ENABLE_TIMING_ASSERTIONS=${global_timing}
        -DXASSERT_ENABLE_DEBUG=0
        -DCONTROL_EXPECT_ASSERTIONS=${assertions}
        -DCONTROL_EXPECT_TIMING=${timing}
        -DCONTROL_PROBE_${probe}=1
        ${ARGN})
  endforeach()
endmacro()

add_control_profile(GLOBAL_00 0 0 0 0)
add_control_profile(GLOBAL_01 0 1 0 1)
add_control_profile(GLOBAL_10 1 0 1 0)
add_control_profile(GLOBAL_11 1 1 1 1)

add_control_profile(DISABLE_REGULAR 1 1 0 1 -DXASSERT_DISABLE_ASSERTIONS_CONTROL=1)
add_control_profile(DISABLE_TIMING 1 1 1 0 -DXASSERT_DISABLE_TIMING_ASSERTIONS_CONTROL=1)
add_control_profile(DISABLE_BOTH 1 1 0 0
                    -DXASSERT_DISABLE_ASSERTIONS_CONTROL=1
                    -DXASSERT_DISABLE_TIMING_ASSERTIONS_CONTROL=1)
add_control_profile(ENABLE_REGULAR 0 0 1 0 -DXASSERT_ENABLE_ASSERTIONS_CONTROL=1)
add_control_profile(ENABLE_TIMING 0 0 0 1 -DXASSERT_ENABLE_TIMING_ASSERTIONS_CONTROL=1)
add_control_profile(ENABLE_BOTH 0 0 1 1
                    -DXASSERT_ENABLE_ASSERTIONS_CONTROL=1
                    -DXASSERT_ENABLE_TIMING_ASSERTIONS_CONTROL=1)
