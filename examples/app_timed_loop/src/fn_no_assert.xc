// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_ASSERT
#define XASSERT_ENABLE_ASSERTIONS_FN_ASSERT 0 /* Enable assertions*/
#define XASSERT_ENABLE_DEBUG_FN_ASSERT 1    /* Enable printing debug message when asserting */

#include <xs1.h>
#include <xassert.h>
#include <print.h>

void fn_no_assert()
{
    timer t;
    unsigned time;
    t :> time;


    for(int i = 0; i< 5; i++)
    {
        time += (i * 5000);
        t when timerafter(time) :> void;
        xassert_loop_freq("test loop", 10000);
    }
}
