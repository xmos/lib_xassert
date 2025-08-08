// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_ASSERT
#define XASSERT_ENABLE_TIMING_ASSERTIONS_FN_ASSERT 1 /* Enable timing assertions*/
#define XASSERT_ENABLE_DEBUG_FN_ASSERT             1  /* Enable printing debug message when asserting */

#include <xs1.h>
#include <xassert.h>

void fn_assert()
{
    XASSERT_TIMED_BLOCK("test", 1000,
        timer t;
        unsigned time;
        t :> time;
        t when timerafter(time + 5000) :> void;
    );
}
