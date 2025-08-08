// Copyright 2024-2025 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_ASSERT
#define XASSERT_ENABLE_TIMING_ASSERTIONS_FN_ASSERT 1 /* Enable timing assertions*/
#define XASSERT_ENABLE_DEBUG_FN_ASSERT             1 /* Enable printing debug message when asserting */
#define XASSERT_ENABLE_LINE_NUMBERS                1 /* Enable line numbers in assertions */
#include <xs1.h>
#include <xassert.h>

void fn_assert()
{
    timer t;
    unsigned time;
    t :> time;

    for(int i = 0; i< 5; i++)
    {
        int delay = 1000;

        xassert_loop_freq("test loop", 10000);

        if(i == 3)
        {
            delay *= 50;

            /* Due to this added exception the assertion will not fail */
            xassert_loop_exception("test loop");
        }

        time += delay;
        t when timerafter(time) :> void;
    }
}
