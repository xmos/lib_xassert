// Copyright 2024-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#define XASSERT_UNIT FN_ASSERT
#define XASSERT_ENABLE_ASSERTIONS_FN_ASSERT 0 /* Enable assertions*/
#define XASSERT_ENABLE_DEBUG_FN_ASSERT 1    /* Enable printing debug message when asserting */

#include <xs1.h>
#include <xassert.h>
#include <basix/print.h>
#include <xcore/hwtimer.h>

void fn_no_assert()
{
    hwtimer_t t = hwtimer_alloc();
    unsigned time = hwtimer_get_time(t);

    for(int i = 0; i< 5; i++)
    {
        time += (i * 5000);
        hwtimer_wait_until(t, time);
        xassert_loop_freq("test loop", 10000);
    }
}
